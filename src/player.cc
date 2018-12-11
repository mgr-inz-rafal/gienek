#include "player.hpp"
#include "doom_controller.hpp"
#include "doommap.hpp"
#include "player_states.hpp"
#include "toolbox.hpp"

#include <chrono>
#include <fstream>
#include <thread>

namespace gienek {

player::player(gienek::doommap& map, gienek::doom_controller& doom_controller)
    : _map(map)
    , _doom_controller(doom_controller)
    , _state(player_states::IDLE)
    , _state_implementation(new IdlePlayerState) {
    _path.set_map(_map);
}

void player::set_state(player_states state) {
    _state = state;

    // TODO: Introduce factory
    switch (state) {
        case player_states::IDLE:
            _state_implementation = std::unique_ptr<BasePlayerState>(new IdlePlayerState);
            break;
        case player_states::MOVING_TO:
            _state_implementation = std::unique_ptr<BasePlayerState>(new MovingToPlayerState);
            break;
        case player_states::ROTATING_TO:
            _state_implementation = std::unique_ptr<BasePlayerState>(new RotatingToPlayerState);
            break;
    }
};

void player::go_to(point<int16_t> target) {
    _target = target;
    _path.calculated = false;
    set_state(player_states::MOVING_TO);
}

void player::rotate_to(point<int16_t> target) {
    _target = target;
    _path.calculated = false;
    set_state(player_states::ROTATING_TO);
}

std::optional<point<int16_t>> player::get_target() const {
    return _target;
}

std::optional<point<int16_t>> player::get_next_route_point() const {
    if ((!_path.calculated) || (is_player_at_final_destination())) {
        return std::nullopt;
    }

    point<int16_t> pt{ static_cast<int16_t>(_next_target_point.value()->x),
                       static_cast<int16_t>(_next_target_point.value()->y) };
    return pt;
}

const BasePlayerState& player::get_state() const {
    return *_state_implementation;
}

bool player::adjust_angle() {
    auto angle_target = get_angle_to_rotation_point();
    auto angle_player = _player.angle;
    if (!toolbox::are_doubles_equal(angle_player, angle_target)) {
        switch (toolbox::get_player_turning_direction(angle_player, angle_target)) {
            case player_turning_direction::LEFT:
                _doom_controller.stop_turning_right();
                _doom_controller.start_turning_left();
                break;
            case player_turning_direction::RIGHT:
                _doom_controller.stop_turning_left();
                _doom_controller.start_turning_right();
                break;
        }
    } else {
        _doom_controller.stop_turning();
        return true;
    }
    return false;
}

void player::calculate_path() {
    if (!_target.has_value()) {
        return;
    }
    if (!_path.calculated) {
        _path._route.clear();
        bool correct = _path.calculate(_player.pos, _target.value());
        if (!correct) {
            set_state(player_states::IDLE);
        }
    }
}

bool player::is_player_at_final_destination() const {
    if (!_path.calculated) {
        return true;
    }
    return _path.get_route_points().end() == _next_target_point.value();
}

bool player::set_next_target_point() {
    _next_target_point.value()++;
    return is_player_at_final_destination();
}

void player::operator()() {
    using namespace std::chrono_literals;
    for (;;) {
        switch (_state) {
            case player_states::IDLE:
                break;
            case player_states::ROTATING_TO:
                if (adjust_angle()) {
                    set_state(player_states::MOVING_TO);
                }
                break;
            case player_states::MOVING_TO:
                if (!_path.calculated) {
                    calculate_path();
                    _next_target_point = ++_path.get_route_points().begin();
                }
                bool at_final_destination{ false };
                if (!_next_target_point.has_value()) {
                    _next_target_point = ++_path.get_route_points().begin();
                } else {
                    at_final_destination = set_next_target_point();
                }
                if (at_final_destination) {
                    set_state(player_states::IDLE);
                    _path.reset();
                } else {
                    _target = { static_cast<int16_t>(_next_target_point.value()->x),
                                static_cast<int16_t>(_next_target_point.value()->y) };
                    set_state(player_states::ROTATING_TO);
                }
                break;
        }
    }

    std::this_thread::sleep_for(4ms);
}

actor& player::get_actor() {
    return _player;
}

path& player::get_path() {
    return _path;
}

double player::get_angle_to_next_target_point() const {
    if (!_path.calculated) {
        return 0.0f;
    }
    const point<double> current = { static_cast<double>(_player.pos.x), static_cast<double>(_player.pos.y) };
    const auto& destination = *_next_target_point;

    return toolbox::get_angle_between_points(current, *destination);
}

double player::get_angle_to_rotation_point() const {
    const point<double> current = { static_cast<double>(_player.pos.x), static_cast<double>(_player.pos.y) };
    const point<double> destination = { static_cast<double>(_target->x), static_cast<double>(_target->y) };

    return toolbox::get_angle_between_points(current, destination);
}

} // namespace gienek
