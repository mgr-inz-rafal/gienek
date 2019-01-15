#include "handler_vertex.hpp"

#include <cstdint>
#include <iostream>

namespace gienek {
handler_result handler_vertex::handle() const {
    std::string buffer;
    _reader->read(buffer, get_buffer_size());

    int16_t x, y;
    memcpy(&x, &buffer[0], 2);
    memcpy(&y, &buffer[2], 2);

    _map->add_vertex({ x, y });

    return handler_result{};
};

size_t handler_vertex::get_buffer_size() const {
    return 4;
}

} // namespace gienek
