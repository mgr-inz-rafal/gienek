#include "handler_player_angle.hpp"

#include <string>

namespace gienek {

handler_result handler_player_angle::handle() const {
    std::string buffer;
    _reader->read(buffer, get_buffer_size());

    int16_t angle;
    memcpy(&angle, &buffer[0], 2);

    {
        std::lock_guard guard(_map->get_map_access_mutex());
        _map->update_player_angle(angle);
        _player->get_actor().angle = angle;
    }

    return handler_result{};
};

size_t handler_player_angle::get_buffer_size() const {
    return 2;
}

} // namespace gienek
