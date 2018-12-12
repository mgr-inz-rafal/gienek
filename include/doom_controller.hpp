#pragma once

#include <string>

#include "types.hpp"

namespace gienek {

class keyboard;

class doom_controller {
    std::string _address;
    keyboard& _keyboard;
    queue_t& _queue;

    // TODO: Figure out some smarter way for this flags to avoid
    // too much copy&paste.
    bool is_turning_right{ false };
    bool is_turning_left{ false };
    bool is_going_forward{ false };

  public:
    doom_controller(const std::string& address, keyboard& keyboard, queue_t& queue);
    void operator()(bool& quit);
    void start_turning_right();
    void start_turning_left();
    void stop_turning_right();
    void stop_turning_left();
    void stop_turning();
    void start_going_forward();
    void stop_going_forward();
};

} // namespace gienek
