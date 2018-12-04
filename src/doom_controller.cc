#include "doom_controller.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

namespace gienek {

doom_controller::doom_controller(const std::string& address)
    : _address(address) {}

void doom_controller::operator()(bool& quit) {
    using namespace std::chrono_literals;

    std::vector<std::string> parts;
    boost::split(parts, _address, [](char c) { return c == ':'; });
    if (2 != parts.size()) {
        throw std::invalid_argument("Correct format for specifying Gienek ZDoom server is SERVERNAME:PORT");
    }

    boost::asio::io_context context;
    boost::asio::ip::tcp::socket gienek_reporting_socket{ context };
    tcp::resolver resolver(context);
    tcp::resolver::results_type endpoints = resolver.resolve(parts.front(), parts.back());
    bool connected = false;
    while (!connected) {
        std::cout << "Attempting to connect to Doom at " << _address << std::endl;
        try {
            boost::asio::connect(gienek_reporting_socket, endpoints);
        } catch (const std::exception& e) {
            std::cerr << "Not connected: " << e.what() << std::endl;
            std::this_thread::sleep_for(1s);
            continue;
        }
        connected = true;
        std::cout << "Connected!" << std::endl;
    };

    while (!quit) {
        std::this_thread::sleep_for(4ms);
    }
}

} // namespace gienek
