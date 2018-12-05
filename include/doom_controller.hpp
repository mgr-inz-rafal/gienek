#pragma once

#include <string>

#include "types.hpp"

namespace gienek {

class keyboard;

class doom_controller {
    std::string _address;
    keyboard& _keyboard;
    queue_t& _queue;

  public:
    doom_controller(const std::string& address, keyboard& keyboard, queue_t& queue);
    void operator()(bool& quit);
};

} // namespace gienek
