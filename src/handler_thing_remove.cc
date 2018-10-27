#include "handler_thing_remove.hpp"

#include <string>

namespace gienek {

handler_result handler_thing_remove::handle() const {
    std::string buffer;
    _reader->read(buffer, 2);

    uint16_t index;

    memcpy(&index, &buffer[0], 2);

    _map->remove_thing(index);

    return handler_result{};
};

} // namespace gienek
