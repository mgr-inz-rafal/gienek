#pragma once

#include "actor.hpp"
#include "player_states.hpp"
#include "point.hpp"

#include <map>

namespace gienek {

class player {
    actor _player;
    point<double> _target;
    player_states _state{ player_states::IDLE };

  public:
    point<double> get_target() const;
    void chase(point<double> target);
};

} // namespace gienek
