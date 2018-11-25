#pragma once

#include <cstdint>

namespace gienek {

class treenode {
  public:
    int16_t parent_index;
    int16_t my_index;
    int16_t my_depth;
};

} // namespace gienek
