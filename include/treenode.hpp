#pragma once

#include <cstdint>
#include <vector>

namespace gienek {

class treenode {
  public:
    treenode* parent;
    int16_t my_index;
    std::vector<treenode> leafs;
};

} // namespace gienek
