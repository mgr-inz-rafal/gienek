#include "socket_reader.hpp"

#include <iostream>

using boost::asio::ip::tcp;

namespace gienek {

socket_reader::socket_reader(tcp::socket& socket)
    : _socket(socket) {}

void socket_reader::read(std::string& buffer, std::size_t size) {
    buffer.resize(size);

    boost::system::error_code incoming_code;
    boost::asio::read(_socket, boost::asio::buffer(buffer), boost::asio::transfer_at_least(buffer.size()),
                      incoming_code);
    if (incoming_code.value()) {
        throw std::runtime_error(incoming_code.message());
    }
}

} // namespace gienek
