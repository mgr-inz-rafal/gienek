#include "sector.hpp"

namespace gienek {

sector::sector(int16_t floor_height, int16_t ceiling_height)
    : _floor_height(floor_height)
    , _ceiling_height(ceiling_height) {}

int16_t sector::get_floor_height() const {
    return _floor_height;
}

int16_t sector::get_ceiling_height() const {
    return _ceiling_height;
}

} // namespace gienek
