#include "handler_thing.hpp"

#include <string>

namespace gienek {

handler_result handler_thing::handle() const {
    std::string buffer;
    _reader->read(buffer, 12);

    thing t;

    memcpy(&t.index, &buffer[0], 2);
    memcpy(&t.health, &buffer[2], 2);
    memcpy(&t.direction, &buffer[4], 2);
    memcpy(&t.posx, &buffer[6], 2);
    memcpy(&t.posy, &buffer[8], 2);
    memcpy(&t.posz, &buffer[10], 2);

    _map->add_thing(std::move(t));

    return handler_result{};
};

} // namespace gienek
