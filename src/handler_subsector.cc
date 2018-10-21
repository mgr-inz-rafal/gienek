#include "handler_subsector.hpp"
#include "doommap.hpp"

#include <cstdint>
#include <iostream>

namespace gienek {
handler_result handler_subsector::handle() const {
    // std::cout << "Subsector - ";

    std::string buffer;
    _reader->read(buffer, 2);

    int16_t count;
    memcpy(&count, &buffer[0], 2);

    // std::cout << "Number of segs: " << count << std::endl;
    gienek::subsector& ss = _map->add_subsector();

    for (int16_t i = 0; i < count; ++i) {
        _reader->read(buffer, 4);
        int16_t sti, eti;
        memcpy(&sti, &buffer[0], 2);
        memcpy(&eti, &buffer[2], 2);
        // std::cout << "\tSeg #" << i + 1 << ": vertex #" << sti << " to #" << eti << std::endl;
        ss.segs.push_back({ sti, eti });
    }

    return handler_result{};
};
} // namespace gienek
