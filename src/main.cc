#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <boost/asio.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "decoder.hpp"
#include "display_config.hpp"
#include "doom_controller.hpp"
#include "event_loop.hpp"
#include "handler.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "painter.hpp"
#include "player.hpp"
#include "point.hpp"
#include "scaler.hpp"
#include "socket_reader.hpp"
#include "toolbox.hpp"
#include "user_interactions.hpp"

using boost::asio::ip::tcp;

ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_DISPLAY* display = NULL;

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

int main() {
    using namespace std::chrono_literals;
    const unsigned int WINDOW_WIDTH = 1024;
    const unsigned int WINDOW_HEIGHT = 768;

    gienek::display_config discfg{ WINDOW_WIDTH, WINDOW_HEIGHT };
    gienek::user_interactions user_interactions;
    gienek::doommap map(discfg, user_interactions);
    gienek::scaler scaler(discfg);
    gienek::toolbox::_map = &map;
    gienek::toolbox::_scaler = &scaler;

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

    bool exit_application = false;

    gienek::doom_controller doom;
    gienek::keyboard keyboard;
    gienek::mouse mouse;
    gienek::player slayer{ map };
    gienek::decoder decoder(slayer);
    gienek::painter painter{ map, slayer, mouse, keyboard, scaler, user_interactions };
    std::thread drawer(std::ref(painter), std::ref(exit_application), event_queue);
    std::thread player_ai(std::ref(slayer));
    std::thread doom_controller(std::ref(doom), std::ref(exit_application));

    try {
        boost::asio::io_context context;
        tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), 13));

        gienek::event_loop loop(slayer, mouse, keyboard, painter, map, user_interactions, event_queue, scaler);
        std::thread mainloop(std::ref(loop), std::ref(context), std::ref(exit_application));

        std::cout << "Awaiting connection..." << std::endl;
        tcp::socket socket(context);
        acceptor.async_accept(socket, [&](const boost::system::error_code& error) {
            if (!error) {
                std::cout << "Connection established, waiting for commands..." << std::endl;
                gienek::socket_reader sr(socket);
                while (!exit_application) {
                    unsigned char command = get_command(sr);
                    if (' ' != command) {
                        auto handler = decoder.get_handler(command);

                        handler->set_socket_reader(sr);
                        handler->set_doommap(map);
                        handler->set_player(slayer);
                        auto result = handler->handle();
                        if (result.disconnect) {
                            std::cout << "Closing connection on request by client" << std::endl;
                            break;
                        }
                    } else {
                        // TODO: This sleep must be removed and async_read()
                        // must be used in socket_reader().
                        std::this_thread::sleep_for(4ms);
                    }
                }
            }
        });
        context.run();
        drawer.join();
        mainloop.join();
        player_ai.join();
        doom_controller.join();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
