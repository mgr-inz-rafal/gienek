#pragma once

#include <cstdint>
#include <string>

namespace gienek {

class thing {
  public:
    uint16_t index;
    int16_t health;
    int16_t direction;
    int16_t posx;
    int16_t posy;
    int16_t posz;
    int16_t type;

  public:
    std::string get_tag() const;
};

} // namespace gienek
