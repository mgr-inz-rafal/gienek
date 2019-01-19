#pragma once

#include <chrono>
#include <string>

#include "types.hpp"

namespace gienek {

class keyboard;

class doom_controller {
    std::string _address;
    keyboard& _keyboard;
    queue_t& _queue;

    // TODO: Extract to specialized class
    enum class use_status_t { INITIAL_DELAY, COOLDOWN, IDLE };
    use_status_t use_status{ use_status_t::IDLE };
    std::chrono::time_point<std::chrono::system_clock> last_use;
    bool can_perform_use() const;
    // ------------------------------------

    // TODO: Figure out some smarter way for this flags to avoid
    // too much copy&paste.
    bool is_turning_right{ false };
    bool is_turning_left{ false };
    bool is_going_forward{ false };

  public:
    doom_controller(const std::string& address, keyboard& keyboard, queue_t& queue);
    void operator()(bool& quit);
    void perform_use_with_cooldown();

    void start_turning_right();
    void start_turning_left();
    void stop_turning_right();
    void stop_turning_left();
    void stop_turning();
    void start_going_forward();
    void stop_going_forward();
    void start_use();
    void stop_use();
};

} // namespace gienek
