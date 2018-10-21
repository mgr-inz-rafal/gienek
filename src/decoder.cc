#include "decoder.hpp"
#include "cmd_types.hpp"
#include "handler.hpp"
#include "handler_clear.hpp"
#include "handler_map_received.hpp"
#include "handler_quit.hpp"
#include "handler_subsector.hpp"
#include "handler_unknown.hpp"
#include "handler_vertex.hpp"

#include <memory>
#include <string>

namespace gienek {

std::unique_ptr<handler> decoder::get_handler(unsigned char input) {
    switch (input) {
        case CMD::VERTEX:
            return std::unique_ptr<handler>(new handler_vertex);
        case CMD::SUBSECTOR:
            return std::unique_ptr<handler>(new handler_subsector);
        case CMD::QUIT:
            return std::unique_ptr<handler>(new handler_quit);
        case CMD::CLEAR:
            return std::unique_ptr<handler>(new handler_clear);
        case CMD::MAP_RECEIVED:
            return std::unique_ptr<handler>(new handler_map_received);
        default:
            return std::unique_ptr<handler>(new handler_unknown);
    }
}

} // namespace gienek
