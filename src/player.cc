#include "player.hpp"
#include "player_states.hpp"

namespace gienek {

void player::chase(point<double> target) {
    _target = target;
    _state = player_states::MOVING_TO;
}

point<double> player::get_target() const {
    return _target;
}

} // namespace gienek
