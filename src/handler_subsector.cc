#include "handler_subsector.hpp"
#include "doommap.hpp"

#include <cstdint>

namespace gienek {
handler_result handler_subsector::handle() const {
    std::string buffer;

    _reader->read(buffer, get_buffer_size());
    int16_t count;
    memcpy(&count, &buffer[0], 2);

    _reader->read(buffer, get_buffer_size());
    int16_t sector;
    memcpy(&sector, &buffer[0], 2);

    gienek::subsector& ss = _map->add_subsector(sector);

    for (int16_t i = 0; i < count; ++i) {
        _reader->read(buffer, 6);
        int16_t sti, eti, linei;
        memcpy(&sti, &buffer[0], 2);
        memcpy(&eti, &buffer[2], 2);
        memcpy(&linei, &buffer[4], 2);
        ss.segs.push_back({ sti, eti, linei });
    }

    ss.calculate_barycenter();

    return handler_result{};
};

size_t handler_subsector::get_buffer_size() const {
    return 2;
}

} // namespace gienek
