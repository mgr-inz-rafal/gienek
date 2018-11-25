#pragma once

#include <cstdint>
#include <list>

namespace gienek {

class treenode {
    treenode* _parent;

  public:
    void set_parent(treenode* parent) { _parent = parent; }
    treenode* get_parent() const { return _parent; };
    int16_t my_index;
    int16_t my_depth;
    std::list<treenode> leafs;
};

} // namespace gienek
