#include "handler_clear.hpp"
#include "player.hpp"
#include "player_states.hpp"

#include <iostream>

namespace gienek {

handler_clear::handler_clear(player& player)
    : _player(player) {}

handler_result handler_clear::handle() const {
    std::cout << "Clear map data" << std::endl;

    _player.set_state(player_states::IDLE);
    _player.get_path().calculated = false;
    _map->clear();

    return handler_result{};
};

} // namespace gienek
