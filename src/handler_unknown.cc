#include "handler_unknown.hpp"

#include <iostream>

namespace gienek {

handler_result handler_unknown::handle() const {
    std::cout << "Unknown!" << std::endl;

    return handler_result{};
};

} // namespace gienek
