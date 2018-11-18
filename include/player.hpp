#pragma once

#include "actor.hpp"
#include "player_states.hpp"
#include "point.hpp"

#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace gienek {

class path {
  public:
    bool calculated{ false };
    using route_t = std::vector<point<int16_t>>;
    route_t _route;
};

class doommap;

class player {
    actor _player;
    const doommap& _map;
    std::optional<point<int16_t>> _target;
    player_states _state;
    std::unique_ptr<BasePlayerState> _state_implementation;

    void set_state(player_states state);

    // ------------------------------------------
    // TODO: This is to be refactored
    // ------------------------------------------
    path _path;
    // ------------------------------------------

  public:
    explicit player(gienek::doommap& map);
    void operator()();
    actor& get_actor();
    std::optional<point<int16_t>> get_target() const;
    const BasePlayerState& get_state() const;
    void go_to(point<int16_t> target);
};

} // namespace gienek
