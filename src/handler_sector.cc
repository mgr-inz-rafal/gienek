#include "handler_sector.hpp"
#include "doommap.hpp"

namespace gienek {

handler_result handler_sector::handle() const {
    std::string buffer;
    _reader->read(buffer, get_buffer_size());

    int16_t floor;
    int16_t ceiling;
    int16_t tag;
    memcpy(&floor, &buffer[0], 2);
    memcpy(&ceiling, &buffer[2], 2);
    memcpy(&tag, &buffer[4], 2);

    _map->add_sector({ floor, ceiling, tag });

    return handler_result{};
};

size_t handler_sector::get_buffer_size() const {
    return 6;
}

} // namespace gienek
