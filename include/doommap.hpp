#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "display_config.hpp"
#include "line.hpp"
#include "sector.hpp"
#include "subsector.hpp"
#include "thing.hpp"
#include "user_interactions.hpp"
#include "vertex.hpp"

namespace gienek {

class doommap {
    display_config& _display_config;
    bool fully_loaded = false;
    std::vector<vertex> verts;
    std::vector<line> lines;
    std::vector<subsector> ssectors;
    std::vector<sector> sectors;
    std::map<uint16_t, thing> things; // TODO: Should be map to allow trivial udpate_thing()
    user_interactions& _user_interactions;
    uint16_t player_thing_key{ 0 };
    bool can_step_into_subsector(const subsector* src, const subsector* dst, const line* l,
                                 std::optional<int16_t>& teleport_target_sector_tag) const;
    int16_t find_sector_by_tag(int16_t tag) const;
    int16_t find_subsector_with_item(int16_t sector_tag) const;

    mutable std::mutex map_access_mutex;

  public: // TODO: Make private
    std::pair<int16_t, int16_t> min_vertex_coords = { INT16_MAX, INT16_MAX };
    std::pair<int16_t, int16_t> max_vertex_coords = { INT16_MIN, INT16_MIN };

  public:
    doommap(display_config& display_config, user_interactions& user_interactions);
    const line* get_line_from_seg(const seg& s) const;
    void clear();
    bool is_fully_loaded() const;
    void set_fully_loaded();
    void calculate_triangles();
    std::mutex& get_map_access_mutex() const;

    void add_vertex(vertex v);
    void add_line(line l);
    const std::vector<vertex>& get_verts() const;
    const std::vector<line>& get_lines() const;
    subsector& add_subsector(int16_t sector);
    sector& add_sector(const sector& _sector);
    const std::vector<subsector>& get_ssectors() const;
    const std::vector<sector>& get_sectors() const;
    const std::vector<std::int16_t> get_adjacent_subsectors(const subsector* ss) const;
    void add_thing(thing t);
    void update_thing(thing t);
    void remove_thing(uint16_t index);
    const std::map<uint16_t, thing>& get_things() const;
    void update_player_angle(int16_t angle);
    std::optional<std::reference_wrapper<const seg>> get_seg_between_subsectors(int16_t first, int16_t second) const;
    point<double> get_middle_point_of_seg(const seg& s) const;
    void update_sector_ceiling_height(int16_t sector_index, int16_t new_height);
    void update_sector_floor_height(int16_t sector_index, int16_t new_height);
};

} // namespace gienek
