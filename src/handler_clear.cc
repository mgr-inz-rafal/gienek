#include "handler_clear.hpp"

#include <iostream>

namespace gienek {
handler_result handler_clear::handle() const {
    std::cout << "Clear map data" << std::endl;

    _map->clear();

    return handler_result{};
};

} // namespace gienek
