#pragma once

#include <cstdint>
#include <list>

namespace gienek {

class treenode {
  public:
    int16_t parent_index;
    int16_t my_index;
    int16_t my_depth;
    std::list<treenode> leafs;
};

} // namespace gienek
