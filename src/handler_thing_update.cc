#include "handler_thing_update.hpp"

#include <string>

namespace gienek {

handler_result handler_thing_update::handle() const {
    std::string buffer;
    _reader->read(buffer, get_buffer_size());

    thing t;

    memcpy(&t.index, &buffer[0], 2);
    memcpy(&t.health, &buffer[2], 2);
    memcpy(&t.direction, &buffer[4], 2);
    memcpy(&t.posx, &buffer[6], 2);
    memcpy(&t.posy, &buffer[8], 2);
    memcpy(&t.posz, &buffer[10], 2);
    memcpy(&t.type, &buffer[12], 2);

    {
        std::lock_guard guard(_map->get_map_access_mutex());
        if (29999 == t.type) {
            _player->get_actor().pos = { t.posx, t.posy };
            _player->get_actor().angle = t.direction;
        }
        _map->update_thing(std::move(t));
    }

    return handler_result{};
};

size_t handler_thing_update::get_buffer_size() const {
    return 14;
}

} // namespace gienek
