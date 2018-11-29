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
    std::list<treenode> flooded;
    std::set<int16_t> visited_subsectors;
    bool is_visited(int16_t index);
    bool generate_children(treenode& node, int16_t target_ssector, treenode*& target_node);
    std::vector<point<double>> points;

  public:
    void set_map(const doommap& map) { _map = &map; }
    bool calculated{ false };
    using route_t = std::vector<point<int16_t>>;
    route_t _route;
    bool calculate(point<int16_t> start, point<int16_t> end);
    std::list<int16_t> get_route_subsectors() const; // TODO: Keep precalculated
    std::vector<point<double>> get_route_points() const;
};
} // namespace gienek
