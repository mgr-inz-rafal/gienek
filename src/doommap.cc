#include "doommap.hpp"

namespace gienek {

doommap::doommap(display_config& display_config)
    : _display_config(display_config)
    , fully_loaded(false){};

void doommap::add_vertex(vertex v) {
    verts.push_back(v);
    if (v.x < min_vertex_coords.first) {
        _display_config.SCALE = -1.0f; // Force scale recalculation
        min_vertex_coords.first = v.x;
    }
    if (v.y < min_vertex_coords.second) {
        _display_config.SCALE = -1.0f; // Force scale recalculation
        min_vertex_coords.second = v.y;
    }
    if (v.x > max_vertex_coords.first) {
        _display_config.SCALE = -1.0f; // Force scale recalculation
        max_vertex_coords.first = v.x;
    }
    if (v.y > max_vertex_coords.second) {
        _display_config.SCALE = -1.0f; // Force scale recalculation
        max_vertex_coords.second = v.y;
    }
}

subsector& doommap::add_subsector() {
    ssectors.push_back({});
    return ssectors.back();
}

void doommap::clear() {
    std::lock_guard guard(map_access_mutex);
    ssectors.clear();
    verts.clear();
    fully_loaded = false;
    _display_config.SCALE = -1.0f; // Force scale recalculation
    min_vertex_coords = { INT16_MAX, INT16_MAX };
    max_vertex_coords = { INT16_MIN, INT16_MIN };
}

void doommap::calculate_triangles() {
    for (auto&& ssector : ssectors) {
        seg_triangle tr;
        tr.parent = &ssector;
        tr.coords[0] = verts[ssector.segs[0].sti];
        for (std::size_t i = 1; i < ssector.segs.size() - 1; ++i) {
            tr.coords[1] = verts[ssector.segs[i - 1].eti];
            tr.coords[2] = verts[ssector.segs[i].eti];
            ssector.triangles.emplace_back(tr);
        }
    }
}

bool doommap::is_fully_loaded() const {
    return fully_loaded;
}

void doommap::set_fully_loaded() {
    fully_loaded = true;
}

const std::vector<vertex>& doommap::get_verts() const {
    return verts;
}

const std::vector<subsector>& doommap::get_ssectors() const {
    return ssectors;
}

} // namespace gienek
