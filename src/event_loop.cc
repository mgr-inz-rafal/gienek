#include <boost/asio.hpp>

#include "event_loop.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "painter.hpp"
#include "player.hpp"
#include "user_interactions.hpp"

namespace gienek {

event_loop::event_loop(gienek::player& player, gienek::mouse& mouse, gienek::keyboard& keyboard,
                       gienek::painter& painter, gienek::doommap& map, gienek::user_interactions& user_interactions,
                       ALLEGRO_EVENT_QUEUE* event_queue)
    : _player(player)
    , _mouse(mouse)
    , _keyboard(keyboard)
    , _painter(painter)
    , _map(map)
    , _user_interactions(user_interactions)
    , _event_queue(event_queue){};

void event_loop::operator()(boost::asio::io_context& context, bool& quit) {
    for (;;) {
        if (!al_event_queue_is_empty(_event_queue)) {
            al_get_next_event(_event_queue, &_event);
            if (ALLEGRO_EVENT_MOUSE_BUTTON_UP == _event.type) {
                _mouse.mouse_click.x = static_cast<double>(_event.mouse.x);
                _mouse.mouse_click.y = static_cast<double>(_event.mouse.y);
                _user_interactions.set_clicked_traingle(
                    gienek::toolbox::determine_clicked_triangle(_mouse, _painter.get_scaler(), _map));

                // Go to
                if (_keyboard.keystate[ALLEGRO_KEY_G]) {
                    _player.chase(_mouse.mouse_click);
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
    }
}
} // namespace gienek
