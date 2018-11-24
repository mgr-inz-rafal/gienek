#pragma once

#include <memory>
#include <string>

namespace gienek {

class handler;
class player;

class decoder {
    player& _player;

  public:
    decoder(player& player);
    std::unique_ptr<handler> get_handler(unsigned char input);
};

} // namespace gienek
