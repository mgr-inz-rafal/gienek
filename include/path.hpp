#pragma once

#include "path.hpp"
#include "point.hpp"
#include "treenode.hpp"

#include <cstdint>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace gienek {

class doommap;

class path {
    const doommap* _map;
    treenode* target = nullptr;
    std::list<treenode> all_nodes;
    treenode root;
    treenode* found;
    std::list<treenode> flooded;

  public:
    // ------------------------------------------
    // TODO: This is to be refactored
    // ------------------------------------------
    bool generate_children(treenode& node, int16_t target_ssector, treenode*& target_node);
    std::set<int16_t> visited_subsectors;
    bool is_visited(int16_t index) { return visited_subsectors.end() != visited_subsectors.find(index); };
    // ------------------------------------------

  public:
    void set_map(const doommap& map) { _map = &map; }
    bool calculated{ false };
    using route_t = std::vector<point<int16_t>>;
    route_t _route;
    bool calculate(point<int16_t> start, point<int16_t> end);
    std::list<int16_t> get_route_elements();
};
} // namespace gienek
