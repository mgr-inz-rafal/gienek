#include "event_loop.hpp"
#include "mouse.hpp"
#include "painter.hpp"
#include "user_interactions.hpp"

namespace gienek {

event_loop::event_loop(gienek::mouse& mouse, gienek::painter& painter, gienek::doommap& map,
                       gienek::user_interactions& user_interactions, ALLEGRO_EVENT_QUEUE* event_queue)
    : _mouse(mouse)
    , _painter(painter)
    , _map(map)
    , _user_interactions(user_interactions)
    , _event_queue(event_queue){};

void event_loop::operator()(bool& quit) {
    while (!quit) {
        if (!al_event_queue_is_empty(_event_queue)) {
            al_get_next_event(_event_queue, &_event);
            if (ALLEGRO_EVENT_MOUSE_BUTTON_UP == _event.type) {
                _mouse.mouse_click.x = static_cast<double>(_event.mouse.x);
                _mouse.mouse_click.y = static_cast<double>(_event.mouse.y);
                _user_interactions.set_clicked_traingle(
                    gienek::toolbox::determine_clicked_triangle(_mouse, _painter.get_scaler(), _map));
            } else if (ALLEGRO_EVENT_DISPLAY_CLOSE == _event.type) {
                quit = true;
            }
        }
    }
}
} // namespace gienek
