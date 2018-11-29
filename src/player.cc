#include "player.hpp"
#include "doommap.hpp"
#include "player_states.hpp"
#include "toolbox.hpp"

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

const BasePlayerState& player::get_state() const {
    return *_state_implementation;
}

void player::operator()() {
    for (;;) {
        switch (_state) {
            case player_states::IDLE:
                break;
            case player_states::MOVING_TO:
                std::lock_guard guard(path_access_mutex);
                if (!_path.calculated) {
                    _path._route.clear();
                    bool correct = _path.calculate(_player.pos, _target.value());
                    if (!correct) {
                        set_state(player_states::IDLE);
                    }
                }
                break;
        }
    }
}

actor& player::get_actor() {
    return _player;
}

path& player::get_path() {
    return _path;
}

} // namespace gienek
