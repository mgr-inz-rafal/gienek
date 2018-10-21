#include "handler_quit.hpp"

#include <iostream>

namespace gienek {

handler_result handler_quit::handle() const {
    std::cout << "Quit!" << std::endl;

    return handler_result{ 1 };
};

} // namespace gienek
