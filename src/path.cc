#include "path.hpp"
#include "doommap.hpp"
#include "toolbox.hpp"
#include "treenode.hpp"

namespace gienek {

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
    const auto& str = toolbox::position_to_triangle(start);
    if (!toolbox::is_triangle_ok(str)) {
        return false;
    }
    const auto& etr = toolbox::position_to_triangle(end);
    if (!toolbox::is_triangle_ok(etr)) {
        return false;
    }

    visited_subsectors.clear();
    visited_subsectors.insert(static_cast<int16_t>(str.first));

    root.parent_index = -1;
    root.my_index = static_cast<int16_t>(str.first);
    root.my_depth = 0;
    all_nodes.clear();
    all_nodes.push_back(root);
    flooded.push_back(root);

    while (!flooded.empty()) {
        treenode flood = flooded.front();
        flooded.pop_front();
        generate_children(flood, static_cast<int16_t>(etr.first), target);
    }

    calculated = true;
    return true;
}

std::list<int16_t> path::get_route_elements() const {
    std::list<int16_t> ret;
    if (!calculated) {
        return ret;
    }

    const treenode* tmp = target;
    for (;;) {
        ret.push_front(tmp->my_index);
        int16_t parent = tmp->parent_index;
        if (parent == -1) {
            break;
        }

        tmp = &(*std::find_if(all_nodes.begin(), all_nodes.end(),
                              [parent](const treenode& node) { return node.my_index == parent; }));
    }
    return ret;
}

bool path::is_visited(int16_t index) {
    return visited_subsectors.end() != visited_subsectors.find(index);
}

} // namespace gienek
