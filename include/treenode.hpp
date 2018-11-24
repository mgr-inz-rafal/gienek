#pragma once

#include <cstdint>
#include <vector>

namespace gienek {

class treenode {
  public:
    int16_t parent;
    int16_t my_index;
    std::vector<treenode> leafs;
};

} // namespace gienek
