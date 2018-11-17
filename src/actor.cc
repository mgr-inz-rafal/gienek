#include "actor.hpp"

namespace gienek {

void actor::set_action(actor_actions act, bool onoff) {
    actions[act] = onoff;
}

} // namespace gienek
