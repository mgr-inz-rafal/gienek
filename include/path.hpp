#pragma once

#include "path.hpp"
#include "point.hpp"

#include <cstdint>
#include <set>
#include <vector>

namespace gienek {

class doommap;
class treenode;

class path {
    const doommap* _map;

  public:
    // ------------------------------------------
    // TODO: This is to be refactored
    // ------------------------------------------
    bool generate_children(treenode& node, int16_t target_ssector, treenode*& target_node);
    std::set<int16_t> visited_subsectors;
    // ------------------------------------------

  public:
    void set_map(const doommap& map) { _map = &map; }
    bool calculated{ false };
    using route_t = std::vector<point<int16_t>>;
    route_t _route;
    void calculate(point<int16_t> start, point<int16_t> end);
};
} // namespace gienek
