#pragma once

#include <cstdint>

namespace gienek {

class sector {
    int16_t _floor_height;
    int16_t _ceiling_height;

  public:
    sector(int16_t floor_height, int16_t ceiling_height);
    int16_t get_floor_height() const;
    int16_t get_ceiling_height() const;
};

} // namespace gienek
