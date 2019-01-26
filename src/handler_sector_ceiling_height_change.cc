#include "handler_sector_ceiling_height_change.hpp"
#include "doommap.hpp"

namespace gienek {

handler_result handler_sector_ceiling_height_change::handle() const {
    std::string buffer;
    _reader->read(buffer, get_buffer_size());

    int16_t index;
    int16_t ceiling;
    memcpy(&index, &buffer[0], 2);
    memcpy(&ceiling, &buffer[2], 2);

    _map->update_sector_ceiling_height(index, ceiling);

    return handler_result{};
};

size_t handler_sector_ceiling_height_change::get_buffer_size() const {
    return 4;
}

} // namespace gienek
