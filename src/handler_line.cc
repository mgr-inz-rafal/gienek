#include "handler_line.hpp"

#include <cstdint>
#include <iostream>

namespace gienek {
handler_result handler_line::handle() const {
    std::string buffer;
    _reader->read(buffer, BUFFER_SIZE);

    int16_t x1, y1, x2, y2, tag, type, activation;
    memcpy(&x1, &buffer[0], 2);
    memcpy(&y1, &buffer[2], 2);
    memcpy(&x2, &buffer[4], 2);
    memcpy(&y2, &buffer[6], 2);
    memcpy(&tag, &buffer[8], 2);
    memcpy(&type, &buffer[10], 2);
    memcpy(&activation, &buffer[12], 2);

    _map->add_line({ x1, y1, x2, y2, tag, type, activation });

    return handler_result{};
};
} // namespace gienek
