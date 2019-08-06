#include "doommap.hpp"
#include "toolbox.hpp"

#include <iostream>

namespace gienek {

doommap::doommap(display_config& display_config, user_interactions& user_interactions)
    : _display_config(display_config)
    , _user_interactions(user_interactions)
    , fully_loaded(false){};

void doommap::add_line(line l) {
    lines.emplace_back(l);
}

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

subsector& doommap::add_subsector(int16_t sector) {
    ssectors.push_back({ verts, sector });
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
    sectors.clear();
    verts.clear();
    things.clear();
    lines.clear();
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

const std::vector<line>& doommap::get_lines() const {
    return lines;
}

const std::vector<subsector>& doommap::get_ssectors() const {
    return ssectors;
}

const std::vector<sector>& doommap::get_sectors() const {
    return sectors;
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

bool doommap::can_step_into_subsector(const subsector* src, const subsector* dst, const line* l,
                                      std::optional<int16_t>& teleport_target_sector_tag) const {
    // Check floor heights
    auto src_floor = sectors[src->get_parent_sector()].get_floor_height();
    auto dst_floor = sectors[dst->get_parent_sector()].get_floor_height();
    if (dst_floor - src_floor > 24) {
        // Step too high, maybe it's a lift?
        return toolbox::is_doom_platform(l);
    }

    // Check minimum height of the sector
    auto dst_ceiling = sectors[dst->get_parent_sector()].get_ceiling_height();
    if (dst_ceiling - dst_floor < 56) {
        // Ceiling too low, check if this is a door
        return toolbox::is_doom_door(l);
    }

    // Check if we try to step through teleport
    if (toolbox::is_doom_teleport(l)) {
        teleport_target_sector_tag = l->tag;
    }

    return true;
}

const line* doommap::get_line_from_seg(const seg& s) const {
    return (-1 == s.linei) ? nullptr : &lines[s.linei];
}

int16_t doommap::find_sector_by_tag(int16_t tag) const {
    for (int16_t i = 0; i < static_cast<int16_t>(sectors.size()); ++i) {
        if (sectors[i].get_tag() == tag) {
            return i;
        }
    }
    throw std::runtime_error((boost::format("Stumbled upon teleport to non - existing sector tag(%1%)") % tag).str());
}

int16_t doommap::find_subsector_with_item(int16_t sector_tag) const {
    return 7;
}

const std::vector<adjacent_sector> doommap::get_adjacent_subsectors(const subsector* ss) const {
    std::vector<adjacent_sector> result;
    std::size_t index = 0;
    for (std::size_t index = 0; index < ssectors.size(); ++index) {
        if (&ssectors[index] == ss) {
            continue;
        }
        for (const auto& seg1 : ssectors[index].segs) {
            int index1 = -1;
            for (const auto& seg2 : ss->segs) {
                ++index1;
                if (seg1 == seg2) {
                    auto line = get_line_from_seg(seg1);
                    std::optional<int16_t> target_teleport_tag;
                    if (can_step_into_subsector(ss, &ssectors[index], line, target_teleport_tag)) {
                        result.push_back({ static_cast<int16_t>(index), false });
                        if (target_teleport_tag) {
                            const auto teleport_destination_sector = find_sector_by_tag(*target_teleport_tag);
                            const auto subsector_with_teleport_destination =
                                find_subsector_with_item(teleport_destination_sector);
                            result.push_back({ subsector_with_teleport_destination, true, seg1 });
                        }
                    }
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

void doommap::update_sector_ceiling_height(int16_t sector_index, int16_t new_height) {
    sectors[sector_index].set_ceiling_height(new_height);
}

void doommap::update_sector_floor_height(int16_t sector_index, int16_t new_height) {
    sectors[sector_index].set_floor_height(new_height);
}

} // namespace gienek
