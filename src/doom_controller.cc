#include "doom_controller.hpp"
#include "keyboard.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

namespace gienek {

doom_controller::doom_controller(const std::string& address, keyboard& keyboard, queue_t& queue)
    : _address(address)
    , _keyboard(keyboard)
    , _queue(queue) {}

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

        if (_keyboard.keystate[ALLEGRO_KEY_L]) {
            if (_keyboard.keystate[ALLEGRO_KEY_LSHIFT] || _keyboard.keystate[ALLEGRO_KEY_RSHIFT]) {
                _queue.enq('l');
            } else {
                _queue.enq('L');
            }
        }
        if (_keyboard.keystate[ALLEGRO_KEY_R]) {
            if (_keyboard.keystate[ALLEGRO_KEY_LSHIFT] || _keyboard.keystate[ALLEGRO_KEY_RSHIFT]) {
                _queue.enq('r');
            } else {
                _queue.enq('R');
            }
        }
        if (_keyboard.keystate[ALLEGRO_KEY_F]) {
            if (_keyboard.keystate[ALLEGRO_KEY_LSHIFT] || _keyboard.keystate[ALLEGRO_KEY_RSHIFT]) {
                _queue.enq('f');
            } else {
                _queue.enq('F');
            }
        }
        if (_keyboard.keystate[ALLEGRO_KEY_B]) {
            if (_keyboard.keystate[ALLEGRO_KEY_LSHIFT] || _keyboard.keystate[ALLEGRO_KEY_RSHIFT]) {
                _queue.enq('b');
            } else {
                _queue.enq('B');
            }
        }
        if (_keyboard.keystate[ALLEGRO_KEY_A]) {
            if (_keyboard.keystate[ALLEGRO_KEY_LSHIFT] || _keyboard.keystate[ALLEGRO_KEY_RSHIFT]) {
                _queue.enq('a');
            } else {
                _queue.enq('A');
            }
        }

        auto cmd = _queue.deq();
        if (cmd) {
            char c = *cmd;
            boost::system::error_code ignored_error;
            boost::asio::write(doom_control_socket, boost::asio::buffer(&c, 1), ignored_error);
        }

        std::this_thread::sleep_for(4ms);
    }
}

void doom_controller::turn_right(bool start) {
    if (start) {
        _queue.enq('R');
    } else {
        _queue.enq('r');
    }
}

} // namespace gienek
