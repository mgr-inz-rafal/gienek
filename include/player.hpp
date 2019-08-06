#pragma once

#include "actor.hpp"
#include "path.hpp"
#include "player_states.hpp"
#include "player_task.hpp"
#include "point.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <vector>

namespace gienek {

class doommap;
class doom_controller;

class player {
    actor _player;
    doom_controller& _doom_controller;
    doommap& _map; // TODO: Bring back const (dupa)
    std::optional<point<int16_t>> _target;
    std::optional<std::vector<route_component>::const_iterator> _next_target_point;
    player_states _state;
    player_task _task;
    std::unique_ptr<BasePlayerState> _state_implementation;
    bool adjust_angle();
    bool adjust_position();
    void calculate_path();
    bool set_next_target_point(bool& perform_use);
    bool is_player_at_final_destination() const;

    // ------------------------------------------
    // TODO: This is to be refactored
    // ------------------------------------------
    path _path;
    // ------------------------------------------

  public:
    explicit player(gienek::doommap& map, gienek::doom_controller& doom_controller);
    void set_state(player_states state);
    void operator()();
    actor& get_actor();
    std::optional<point<int16_t>> get_target() const;
    std::optional<point<int16_t>> get_next_route_point() const;
    const BasePlayerState& get_state() const;
    void go_to(point<int16_t> target);
    void rotate_to(point<int16_t> target);
    path& get_path();
    double get_angle_to_next_target_point() const;
    double get_angle_to_rotation_point() const;
};

} // namespace gienek
