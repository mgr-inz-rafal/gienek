#pragma once

#include <memory>
#include <string>

namespace gienek {

class handler;

class decoder {
  public:
    std::unique_ptr<handler> get_handler(unsigned char input);
};

} // namespace gienek
