#pragma once

#include "path.hpp"
#include "point.hpp"
#include "route_component.hpp"
#include "treenode.hpp"

#include <array>
#include <cstdint>
#include <list>
#include <map>
#include <mutex>
#include <set>
#include <vector>

namespace gienek {

class doommap;
class line;

class path {
    doommap* _map; // TODO: Bring back const (dupa)
    treenode* target = nullptr;
    std::list<treenode> all_nodes;
    treenode root;
    std::list<treenode> flooded;
    std::set<int16_t> visited_subsectors;
    bool is_visited(int16_t index);
    bool generate_children(treenode& node, int16_t target_ssector, treenode*& target_node);
    std::vector<point<double>> points;
    std::list<int16_t> route_subsectors;
    std::vector<route_component> route_points;
    void calculate_route_subsectors();
    void calculate_route_points();
    std::mutex path_calculation_in_progress;

  public:
    path();
    void reset();
    void set_map(doommap& map) { _map = &map; } // TODO: Bring back const (dupa)
    bool calculated{ false };
    using route_t = std::vector<point<int16_t>>;
    route_t _route;
    bool calculate(point<int16_t> start, point<int16_t> end);
    const std::list<int16_t>& get_route_subsectors() const; // TODO: Keep precalculated
    const std::vector<route_component>& get_route_points() const;
    std::mutex& get_path_calculation_mutex();
};
} // namespace gienek
