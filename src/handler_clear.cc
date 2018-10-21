#include "handler_clear.hpp"

#include <iostream>

extern std::pair<std::size_t, std::size_t> clicked_triangle;

namespace gienek {
handler_result handler_clear::handle() const {
    std::cout << "Clear map data" << std::endl;

    _map->clear();

    clicked_triangle = { std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max() };

    return handler_result{};
};

} // namespace gienek
