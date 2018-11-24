#include "decoder.hpp"
#include "cmd_types.hpp"
#include "handler.hpp"
#include "handler_clear.hpp"
#include "handler_map_received.hpp"
#include "handler_player_angle.hpp"
#include "handler_quit.hpp"
#include "handler_subsector.hpp"
#include "handler_thing.hpp"
#include "handler_thing_remove.hpp"
#include "handler_thing_update.hpp"
#include "handler_unknown.hpp"
#include "handler_vertex.hpp"

#include <memory>
#include <string>

namespace gienek {

decoder::decoder(player& player)
    : _player(player) {}

std::unique_ptr<handler> decoder::get_handler(unsigned char input) {
    switch (input) {
        case CMD::VERTEX:
            return std::unique_ptr<handler>(new handler_vertex);
        case CMD::SUBSECTOR:
            return std::unique_ptr<handler>(new handler_subsector);
        case CMD::THING:
            return std::unique_ptr<handler>(new handler_thing);
        case CMD::THING_UPDATE:
            return std::unique_ptr<handler>(new handler_thing_update);
        case CMD::THING_REMOVE:
            return std::unique_ptr<handler>(new handler_thing_remove);
        case CMD::PLAYER_ANGLE:
            return std::unique_ptr<handler>(new handler_player_angle);
        case CMD::QUIT:
            return std::unique_ptr<handler>(new handler_quit);
        case CMD::CLEAR:
            return std::unique_ptr<handler>(new handler_clear(this->_player));
        case CMD::MAP_RECEIVED:
            return std::unique_ptr<handler>(new handler_map_received);
        default:
            return std::unique_ptr<handler>(new handler_unknown);
    }
}

} // namespace gienek
