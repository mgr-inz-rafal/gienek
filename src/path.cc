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
        target_node = &node;
        return true;
    }

    for (std::size_t i = 0; i < children.size(); ++i) {
        if (visited_subsectors.end() == visited_subsectors.find(children[i])) {
            treenode new_node{ &node, static_cast<int16_t>(children[i]) };
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

    root.parent = nullptr;
    root.my_index = static_cast<int16_t>(str.first);

    generate_children(root, static_cast<int16_t>(etr.first), target);
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
        tmp = tmp->parent;
    }
    return ret;
}

} // namespace gienek
