#include "handler.hpp"

namespace gienek {

void handler::set_socket_reader(socket_reader& reader) {
    _reader = &reader;
}

void handler::set_doommap(doommap& map) {
    _map = &map;
}

} // namespace gienek
