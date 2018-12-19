#include "sector.hpp"

namespace gienek {

sector::sector(int16_t floor_height, int16_t ceiling_height)
    : _floor_height(floor_height)
    , _ceiling_height(ceiling_height) {}

} // namespace gienek
