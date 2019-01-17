#include "path.hpp"
#include "doommap.hpp"
#include "toolbox.hpp"
#include "treenode.hpp"

namespace gienek {

path::path() {
    route_points.reserve(4096);
}

bool path::generate_children(treenode& node, int16_t target_ssector, treenode*& target_node) {
    const auto& ss = _map->get_ssectors();
    const auto& subsector = ss[node.my_index];
    const auto& children = _map->get_adjacent_subsectors(&subsector);

    for (int16_t i : children) {
        if (!is_visited(i)) {
            visited_subsectors.insert(i);
            treenode new_node;
            new_node.parent_index = node.my_index;
            new_node.my_depth = node.my_depth + 1;
            new_node.my_index = i;
            auto& emplaced = flooded.emplace_back(new_node);
            all_nodes.push_back(emplaced);
            if (new_node.my_index == target_ssector) {
                target = &all_nodes.back();
            }
        }
    }

    return true;
}

bool path::calculate(point<int16_t> start, point<int16_t> end) {
    std::lock_guard lock(path_calculation_in_progress);
    _route.clear();
    target = nullptr;
    const auto& str = toolbox::position_to_triangle(start);
    if (!toolbox::is_triangle_ok(str)) {
        return false;
    }
    const auto& etr = toolbox::position_to_triangle(end);
    if (!toolbox::is_triangle_ok(etr)) {
        return false;
    }

    // Don't search for path within one subsector
    if (str.first == etr.first) {
        return false;
    }

    visited_subsectors.clear();
    visited_subsectors.insert(static_cast<int16_t>(str.first));

    root.parent_index = -1;
    root.my_index = static_cast<int16_t>(str.first);
    root.my_depth = 0;
    all_nodes.clear();
    all_nodes.push_back(root);
    flooded.clear();
    flooded.push_back(root);

    while (!flooded.empty()) {
        treenode flood = flooded.front();
        flooded.pop_front();
        generate_children(flood, static_cast<int16_t>(etr.first), target);
    }

    if (!target) {
        // No route to destination possible
        return false;
    }
    calculated = true;
    calculate_route_subsectors();
    calculate_route_points();
    return true;
}

void path::calculate_route_subsectors() {
    if (!calculated) {
        return;
    }
    route_subsectors.clear();
    const treenode* tmp = target;
    for (;;) {
        route_subsectors.push_front(tmp->my_index);
        int16_t parent = tmp->parent_index;
        if (parent == -1) {
            break;
        }

        tmp = &(*std::find_if(all_nodes.begin(), all_nodes.end(),
                              [parent](const treenode& node) { return node.my_index == parent; }));
    }
}

const std::list<int16_t>& path::get_route_subsectors() const {
    return route_subsectors;
}

void path::calculate_route_points() {
    if (!calculated) {
        return;
    }
    route_points.clear();
    auto subsectors = _map->get_ssectors();
    auto ssectors = get_route_subsectors();
    auto ss = ssectors.cbegin();
    for (;;) {
        auto first = *ss;
        ++ss;
        if (ssectors.cend() == ss) {
            break;
        }
        auto second = *ss;

        auto pt1 = subsectors[first].get_barycenter();
        auto pt2 = subsectors[second].get_barycenter();

        auto seg = _map->get_seg_between_subsectors(first, second);
        if (seg) {
            const auto line = _map->get_line_from_seg(*seg);

            point<double> pt1d = { static_cast<double>(pt1.x), static_cast<double>(pt1.y) };
            point<double> pt_mid = _map->get_middle_point_of_seg(*seg);
            point<double> pt2d = { static_cast<double>(pt2.x), static_cast<double>(pt2.y) };

            route_points.push_back({ 0, pt1d });
            route_points.push_back({ 0, pt_mid });
            if (line && line->type == 12) { // Door
                route_points.push_back({ 1 });
            }
            route_points.push_back({ 0, pt2d });
        }
    }
}

const std::vector<route_component>& path::get_route_points() const {
    return route_points;
}

bool path::is_visited(int16_t index) {
    return visited_subsectors.end() != visited_subsectors.find(index);
}

std::mutex& path::get_path_calculation_mutex() {
    return path_calculation_in_progress;
}

void path::reset() {
    calculated = false;
}

} // namespace gienek
