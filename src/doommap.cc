#include "doommap.hpp"

namespace gienek {

doommap::doommap(display_config& display_config, user_interactions& user_interactions)
    : _display_config(display_config)
    , _user_interactions(user_interactions)
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
    ssectors.push_back({ verts });
    return ssectors.back();
}

sector& doommap::add_sector(const sector& _sector) {
    sectors.push_back(_sector);
    return sectors.back();
}

void doommap::clear() {
    std::lock_guard guard(map_access_mutex);
    _user_interactions.reset();
    ssectors.clear();
    verts.clear();
    things.clear();
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

std::mutex& doommap::get_map_access_mutex() const {
    return map_access_mutex;
}

void doommap::add_thing(thing t) {
    things.emplace(t.index, std::move(t));
    if (t.type == 29999) {
        player_thing_key = t.index;
    }
}

void doommap::update_thing(thing t) {
    things[t.index] = t;
}

void doommap::remove_thing(uint16_t index) {
    things.erase(index);
}

const std::map<uint16_t, thing>& doommap::get_things() const {
    return things;
}

const std::vector<std::int16_t> doommap::get_adjacent_subsectors(const subsector* ss) const {
    std::vector<std::int16_t> result;
    std::size_t index = 0;
    for (std::size_t index = 0; index < ssectors.size(); ++index) {
        if (&ssectors[index] == ss) {
            continue;
        }
        for (const auto& seg1 : ssectors[index].segs) {
            for (const auto& seg2 : ss->segs) {
                if (seg1 == seg2) {
                    result.push_back(static_cast<int16_t>(index));
                }
            }
        }
    }
    return result;
}

void doommap::update_player_angle(int16_t angle) {
    things[player_thing_key].direction = angle;
}

std::optional<std::reference_wrapper<const seg>> doommap::get_seg_between_subsectors(int16_t first,
                                                                                     int16_t second) const {
    const auto& ss1 = ssectors[first];
    const auto& ss2 = ssectors[second];

    for (const auto& s1 : ss1.segs) {
        for (const auto& s2 : ss2.segs) {
            if (s1 == s2) {
                return s1;
            }
        }
    }

    return std::nullopt;
}

point<double> doommap::get_middle_point_of_seg(const seg& s) const {
    const auto& v1 = verts[s.sti];
    const auto& v2 = verts[s.eti];

    return { (v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f };
}

} // namespace gienek
