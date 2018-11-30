#include <boost/asio.hpp>

#include <chrono>

#include "event_loop.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "painter.hpp"
#include "player.hpp"
#include "scaler.hpp"
#include "user_interactions.hpp"

namespace gienek {

event_loop::event_loop(gienek::player& player, gienek::mouse& mouse, gienek::keyboard& keyboard,
                       gienek::painter& painter, gienek::doommap& map, gienek::user_interactions& user_interactions,
                       ALLEGRO_EVENT_QUEUE* event_queue, const gienek::scaler& scaler)
    : _player(player)
    , _mouse(mouse)
    , _keyboard(keyboard)
    , _painter(painter)
    , _map(map)
    , _user_interactions(user_interactions)
    , _event_queue(event_queue)
    , _scaler(scaler){};

void event_loop::operator()(boost::asio::io_context& context, bool& quit) {
    using namespace std::chrono_literals;
    for (;;) {
        if (!al_event_queue_is_empty(_event_queue)) {
            al_get_next_event(_event_queue, &_event);
            if (ALLEGRO_EVENT_MOUSE_BUTTON_UP == _event.type) {
                _mouse.mouse_click.x = static_cast<double>(_event.mouse.x);
                _mouse.mouse_click.y = static_cast<double>(_event.mouse.y);
                _user_interactions.set_clicked_traingle(gienek::toolbox::position_to_triangle(_mouse));

                // Go to
                if (_keyboard.keystate[ALLEGRO_KEY_G]) {
                    _player.go_to(toolbox::window2map(_mouse.mouse_click));
                }

            } else if (ALLEGRO_EVENT_DISPLAY_CLOSE == _event.type) {
                context.stop();
                quit = true;
                return;
            } else if (ALLEGRO_EVENT_KEY_DOWN == _event.type) {
                _keyboard.keystate[_event.keyboard.keycode] = true;
            } else if (ALLEGRO_EVENT_KEY_UP == _event.type) {
                _keyboard.keystate[_event.keyboard.keycode] = false;
            }
        }
        std::this_thread::sleep_for(4ms);
    }
}
} // namespace gienek
