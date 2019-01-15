#include "handler.hpp"

namespace gienek {

size_t handler::get_buffer_size() const {
    return 0;
}

void handler::set_socket_reader(socket_reader& reader) {
    _reader = &reader;
}

void handler::set_doommap(doommap& map) {
    _map = &map;
}

void handler::set_player(player& player) {
    _player = &player;
}

} // namespace gienek
