#include "handler_map_received.hpp"

#include <iostream>

namespace gienek {

handler_result handler_map_received::handle() const {
    std::cout << "Entire map received!" << std::endl;

    _map->calculate_triangles();
    _map->set_fully_loaded();

    return handler_result{};
};

} // namespace gienek
