#include "handler_sector_floor_height_change.hpp"
#include "doommap.hpp"

namespace gienek {

handler_result handler_sector_floor_height_change::handle() const {
    std::string buffer;
    _reader->read(buffer, get_buffer_size());

    int16_t index;
    int16_t floor;
    memcpy(&index, &buffer[0], 2);
    memcpy(&floor, &buffer[2], 2);

    _map->update_sector_floor_height(index, floor);

    return handler_result{};
};

size_t handler_sector_floor_height_change::get_buffer_size() const {
    return 4;
}

} // namespace gienek
