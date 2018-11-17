#pragma once

#include "actor.hpp"
#include "player_states.hpp"
#include "point.hpp"

#include <map>
#include <memory>

namespace gienek {

class player {
    actor _player;
    point<int16_t> _target{ -1, -1 };
    player_states _state;
    std::unique_ptr<BasePlayerState> _state_implementation;

    void set_state(player_states state);

  public:
    player();
    point<int16_t> get_target() const;
    const BasePlayerState& get_state() const;
    void chase(point<int16_t> target);
};

} // namespace gienek
