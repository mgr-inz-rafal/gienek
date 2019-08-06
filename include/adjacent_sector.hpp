#pragma once

#include <cstdint>

#include "seg.hpp"

namespace gienek {

struct adjacent_sector {
    std::int16_t index;
    bool through_teleport{ false };
    seg teleport_seg;
};

} // namespace gienek
