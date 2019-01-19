#include "doom_controller.hpp"
#include "keyboard.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

namespace gienek {

std::map<int, std::pair<char, char>> keyboard_reactions = {
    { ALLEGRO_KEY_L, { 'l', 'L' } }, { ALLEGRO_KEY_R, { 'r', 'R' } }, { ALLEGRO_KEY_F, { 'f', 'F' } },
    { ALLEGRO_KEY_B, { 'b', 'B' } }, { ALLEGRO_KEY_A, { 'a', 'A' } }, { ALLEGRO_KEY_U, { 'u', 'U' } }
};

doom_controller::doom_controller(const std::string& address, keyboard& keyboard, queue_t& queue)
    : _address(address)
    , _keyboard(keyboard)
    , _queue(queue) {}

bool doom_controller::can_perform_use() const {
    using namespace std::chrono_literals;
    auto current_time = std::chrono::system_clock::now();
    return (current_time - last_use > 2s);
}

void doom_controller::perform_use_with_cooldown() {
    using namespace std::chrono_literals;
    if (can_perform_use()) {
        last_use = std::chrono::system_clock::now();
        start_use();
        std::this_thread::sleep_for(100ms);
        stop_use();
    }
}

void doom_controller::operator()(bool& quit) {
    using namespace std::chrono_literals;
    std::vector<std::string> parts;
    boost::split(parts, _address, [](char c) { return c == ':'; });
    if (2 != parts.size()) {
        throw std::invalid_argument("Correct format for specifying Gienek ZDoom server is SERVERNAME:PORT");
    }

    boost::asio::io_context context;
    boost::asio::ip::tcp::socket doom_control_socket{ context };
    tcp::resolver resolver(context);
    tcp::resolver::results_type endpoints = resolver.resolve(parts.front(), parts.back());
    bool connected = false;
    while (!connected) {
        std::cout << "Attempting to connect to Doom at " << _address << std::endl;
        try {
            boost::asio::connect(doom_control_socket, endpoints);
        } catch (const std::exception& e) {
            std::cerr << "Not connected: " << e.what() << std::endl;
            std::this_thread::sleep_for(1s);
            continue;
        }
        connected = true;
        std::cout << "Connected!" << std::endl;
    };

    while (!quit) {
        // "_queue" is used only here, making it superabundant. But I expect
        // that soon more threads will likely be controlling Doom, so let's keep it and see.
        for (const auto& reaction : keyboard_reactions) {
            if (_keyboard.keystate[reaction.first]) {
                if (_keyboard.keystate[ALLEGRO_KEY_LSHIFT] || _keyboard.keystate[ALLEGRO_KEY_RSHIFT]) {
                    _queue.enq(reaction.second.first);
                } else {
                    _queue.enq(reaction.second.second);
                }
            }
        }

        auto cmd = _queue.deq();
        if (cmd) {
            char c = *cmd;
            boost::system::error_code ignored_error;
            boost::asio::write(doom_control_socket, boost::asio::buffer(&c, 1), ignored_error);
        }

        std::this_thread::sleep_for(1ms);
    }
}

void doom_controller::start_turning_right() {
    if (!is_turning_right) {
        is_turning_right = true;
        _queue.enq('R');
    }
}

void doom_controller::start_turning_left() {
    if (!is_turning_left) {
        is_turning_left = true;
        _queue.enq('L');
    }
}

void doom_controller::stop_turning_right() {
    if (is_turning_right) {
        is_turning_right = false;
        _queue.enq('r');
    }
}
void doom_controller::stop_turning_left() {
    if (is_turning_left) {
        is_turning_left = false;
        _queue.enq('l');
    }
}

void doom_controller::stop_turning() {
    stop_turning_left();
    stop_turning_right();
}

void doom_controller::start_going_forward() {
    if (!is_going_forward) {
        is_going_forward = true;
        _queue.enq('F');
    }
}
void doom_controller::stop_going_forward() {
    if (is_going_forward) {
        is_going_forward = false;
        _queue.enq('f');
    }
}

void doom_controller::start_use() {
    _queue.enq('U');
}

void doom_controller::stop_use() {
    _queue.enq('u');
}

} // namespace gienek
