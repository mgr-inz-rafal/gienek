#include "path.hpp"
#include "doommap.hpp"
#include "toolbox.hpp"
#include "treenode.hpp"

namespace gienek {

bool path::generate_children(treenode& node, int16_t target_ssector, treenode*& target_node) {
    const auto& ss = _map->get_ssectors();
    const auto& subsector = ss[node.my_index];
    const auto& children = _map->get_adjacent_subsectors(&subsector);

    if (node.my_index == target_ssector) {
        candidates.insert({ node.my_depth, &node });
        visited_subsectors.erase(node.my_index);
        return false;
    }

    for (std::size_t i = 0; i < children.size(); ++i) {
        if (visited_subsectors.end() == visited_subsectors.find(children[i])) {
            treenode new_node;
            new_node.my_index = static_cast<int16_t>(children[i]);
            new_node.my_depth = node.my_depth + 1;
            new_node.set_parent(&node);

            visited_subsectors.insert(static_cast<int16_t>(children[i]));
            auto& emplaced = node.leafs.emplace_back(new_node);
            if (generate_children(emplaced, target_ssector, target_node)) {
                return true;
            }
        }
    }

    return false;
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

    root.set_parent(nullptr);
    root.my_index = static_cast<int16_t>(str.first);
    root.my_depth = 0;

    candidates.clear();
    generate_children(root, static_cast<int16_t>(etr.first), target);
    target = candidates.begin()->second;
    calculated = true;
    return true;
}

std::list<int16_t> path::get_route_elements() const {
    std::list<int16_t> ret;
    if (!calculated) {
        return ret;
    }
    treenode* tmp = target;
    while (tmp) {
        ret.push_front(tmp->my_index);
        tmp = tmp->get_parent();
    }
    return ret;
}

} // namespace gienek
