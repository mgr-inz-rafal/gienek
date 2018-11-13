// Commands:
// 'a' - vertex
// 'b' - subsector
// 'x' - clear map data
// 'f' - entire map sent
// 'q' - quit

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <boost/asio.hpp>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "decoder.hpp"
#include "display_config.hpp"
#include "handler.hpp"
#include "mouse.hpp"
#include "painter.hpp"
#include "scaler.hpp"
#include "socket_reader.hpp"
#include "user_interactions.hpp"

using boost::asio::ip::tcp;

ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT ev;

unsigned char get_command(gienek::socket_reader& sr) {
    std::string buffer;
    bool data_available = sr.read(buffer, 1);
    if (!data_available) {
        return ' ';
    }
    if (buffer.size() != 1) {
        throw std::runtime_error("Unexpected transmission (command length != 1)");
    }
    return buffer[0];
}

struct event_loop {
    gienek::mouse& _mouse;
    gienek::painter& _painter;
    gienek::doommap& _map;
    gienek::user_interactions& _user_interactions;
    event_loop(gienek::mouse& mouse, gienek::painter& painter, gienek::doommap& map,
               gienek::user_interactions& user_interactions)
        : _mouse(mouse)
        , _painter(painter)
        , _map(map)
        , _user_interactions(user_interactions) {}
    void operator()() {
        for (;;) {
            if (!al_event_queue_is_empty(event_queue)) {
                al_get_next_event(event_queue, &ev);
                if (ALLEGRO_EVENT_MOUSE_BUTTON_UP == ev.type) {
                    _mouse.mouse_click.x = static_cast<double>(ev.mouse.x);
                    _mouse.mouse_click.y = static_cast<double>(ev.mouse.y);
                    _user_interactions.set_clicked_traingle(
                        gienek::toolbox::determine_clicked_triangle(_mouse, _painter.get_scaler(), _map));
                }
            }
        }
    }
};

int main() {
    const unsigned int WINDOW_WIDTH = 1024;
    const unsigned int WINDOW_HEIGHT = 768;

    gienek::display_config discfg{ WINDOW_WIDTH, WINDOW_HEIGHT };
    gienek::user_interactions user_interactions;
    gienek::doommap map(discfg, user_interactions);
    gienek::scaler scaler(discfg);

    if (!al_init()) {
        std::cout << "al_init() failed" << std::endl;
        return -1;
    }
    if (!al_init_primitives_addon()) {
        std::cout << "al_init_primitives_addon() failed" << std::endl;
        return -1;
    }
    if (!al_install_mouse()) {
        throw std::runtime_error("al_install_mouse() failed");
    }
    if (!al_init_font_addon()) {
        throw std::runtime_error("al_init_font_addon() failed");
    }
    if (!al_init_image_addon()) {
        throw std::runtime_error("al_init_image_addon() failed");
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        throw std::runtime_error("al_create_event_queue() failed");
    }
    al_register_event_source(event_queue, al_get_mouse_event_source());

    if (false == al_install_keyboard()) {
        throw std::runtime_error("al_install_keyboard() failed");
    }
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    gienek::decoder decoder;
    bool exit_application = false;

    gienek::mouse mouse;
    gienek::painter painter{ map, mouse, scaler, user_interactions };
    std::thread drawer(painter, std::ref(exit_application));
    drawer.detach();

    event_loop loop(mouse, painter, map, user_interactions);

    std::thread mainloop(loop);
    mainloop.detach();

    for (;;) {
        try {
            bool quit_current_map = false;

            boost::asio::io_context io_context;
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

            while (!quit_current_map) {
                std::cout << "Awaiting connection..." << std::endl;
                tcp::socket socket(io_context);
                acceptor.accept(socket);
                std::cout << "Connection established, waiting for commands..." << std::endl;
                gienek::socket_reader sr(socket);
                for (;;) {
                    unsigned char command = get_command(sr);
                    if (' ' != command) {
                        auto handler = decoder.get_handler(command);

                        handler->set_socket_reader(sr);
                        handler->set_doommap(map);
                        auto result = handler->handle();
                        if (result.disconnect) {
                            std::cout << "Closing connection on request by client" << std::endl;
                            quit_current_map = true;
                            break;
                        }
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    return 0;
}
