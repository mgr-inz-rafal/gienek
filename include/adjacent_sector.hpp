#pragma once

#include <cstdint>

namespace gienek {

struct adjacent_sector {
    std::int16_t index;
    bool through_teleport{ false };
};

} // namespace gienek
