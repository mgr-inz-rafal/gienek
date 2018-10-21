#include "handler_vertex.hpp"

#include <cstdint>
#include <iostream>

namespace gienek {
handler_result handler_vertex::handle() const {
    //    std::cout << "Vertex - ";

    std::string buffer;
    _reader->read(buffer, 4);

    int16_t x, y;
    memcpy(&x, &buffer[0], 2);
    memcpy(&y, &buffer[2], 2);

    //  std::cout << '(' << x << ',' << y << ')' << std::endl;

    _map->add_vertex({ x, y });

    return handler_result{};
};
} // namespace gienek
