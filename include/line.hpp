#pragma once

#include <cstdint>

namespace gienek {

class line {
  public:
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    int16_t tag;
    int16_t type;
    int16_t activation;
};

}; // namespace gienek
