#pragma once

#include <cstdint>

namespace gienek {

class sector {
    int16_t _floor_height;
    int16_t _ceiling_height;
    int16_t _tag;

  public:
    sector(int16_t floor_height, int16_t ceiling_height, int16_t tag);
    int16_t get_floor_height() const;
    int16_t get_ceiling_height() const;
    void set_ceiling_height(int16_t new_height);
    void set_floor_height(int16_t new_height);
    int16_t get_tag() const;
};

} // namespace gienek
