#include "player.hpp"
#include "player_states.hpp"

namespace gienek {

player::player()
    : _state(player_states::IDLE)
    , _state_implementation(new IdlePlayerState) {}

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

void player::chase(point<int16_t> target) {
    _target = target;
    set_state(player_states::MOVING_TO);
}

std::optional<point<int16_t>> player::get_target() const {
    return _target;
}

const BasePlayerState& player::get_state() const {
    return *_state_implementation;
}

} // namespace gienek
