#pragma once

#include "actor.hpp"
#include "path.hpp"
#include "player_states.hpp"
#include "point.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <vector>

namespace gienek {

class doommap;

class player {
    actor _player;
    const doommap& _map;
    std::optional<point<int16_t>> _target;
    player_states _state;
    std::unique_ptr<BasePlayerState> _state_implementation;

    // ------------------------------------------
    // TODO: This is to be refactored
    // ------------------------------------------
    path _path;
    mutable std::mutex path_access_mutex;
    // ------------------------------------------

  public:
    explicit player(gienek::doommap& map);
    void set_state(player_states state);
    void operator()();
    actor& get_actor();
    std::optional<point<int16_t>> get_target() const;
    const BasePlayerState& get_state() const;
    void go_to(point<int16_t> target);
    path& get_path();
};

} // namespace gienek
