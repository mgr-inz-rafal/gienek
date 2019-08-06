#pragma once

#include "seg.hpp"

#include <cstdint>

namespace gienek {

class treenode {
  public:
    int16_t parent_index;
    int16_t my_index;
    int16_t my_depth;
    bool through_teleport{ false };
    seg teleport_seg;
};

} // namespace gienek
