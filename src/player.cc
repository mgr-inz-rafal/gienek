#include "player.hpp"
#include "doommap.hpp"
#include "player_states.hpp"
#include "toolbox.hpp"

#include <chrono>
#include <thread>

#include <boost/math/constants/constants.hpp>

namespace gienek {

player::player(gienek::doommap& map)
    : _map(map)
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
    }
};

void player::go_to(point<int16_t> target) {
    _target = target;
    _path.calculated = false;
    set_state(player_states::MOVING_TO);
}

std::optional<point<int16_t>> player::get_target() const {
    return _target;
}

std::optional<point<int16_t>> player::get_next_route_point() const {
    if (_path.calculated) {
        point<int16_t> pt{ static_cast<int16_t>(_next_target_point->x), static_cast<int16_t>(_next_target_point->y) };
        return pt;
    }
    return std::nullopt;
}

const BasePlayerState& player::get_state() const {
    return *_state_implementation;
}

void player::operator()() {
    using namespace std::chrono_literals;
    for (;;) {
        switch (_state) {
            case player_states::IDLE:
                break;
            case player_states::MOVING_TO:
                if (!_path.calculated) {
                    _path._route.clear();
                    bool correct = _path.calculate(_player.pos, _target.value());
                    if (!correct) {
                        set_state(player_states::IDLE);
                    }
                    _next_target_point = ++_path.get_route_points().begin();
                }
                break;
        }
        std::this_thread::sleep_for(4ms);
    }
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
    const auto& current = _player.pos;
    const auto& destination = *_next_target_point;

    auto angle = (180.0f * std::atan2(destination.y - current.y, destination.x - current.x)) /
                 boost::math::constants::pi<double>();
    while (angle < 0) {
        angle += 360.0f;
    }
    return std::fmod(angle, 360.0f);
}

} // namespace gienek
