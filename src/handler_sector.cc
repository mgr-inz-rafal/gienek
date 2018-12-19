#include "handler_sector.hpp"
#include "doommap.hpp"

namespace gienek {

handler_result handler_sector::handle() const {
    std::string buffer;
    _reader->read(buffer, 4);

    int16_t floor;
    int16_t ceiling;
    memcpy(&floor, &buffer[0], 2);
    memcpy(&ceiling, &buffer[2], 2);

    _map->add_sector({ floor, ceiling });

    return handler_result{};
};
} // namespace gienek
