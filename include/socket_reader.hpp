#pragma once

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace gienek {

class socket_reader {
    tcp::socket& _socket;

  public:
    socket_reader(tcp::socket& socket);
    void read(std::string& buffer, std::size_t size);
};

} // namespace gienek
