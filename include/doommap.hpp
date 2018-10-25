#pragma once

#include <array>
#include <cstdint>
#include <mutex>
#include <vector>

#include "display_config.hpp"
#include "user_interactions.hpp"

namespace gienek {

struct vertex {
    int16_t x;
    int16_t y;
};

struct seg {
    // Start and end vertex index
    int16_t sti;
    int16_t eti;
};

struct seg_triangle;

struct subsector {
    std::vector<seg> segs;

    // Calculate triangles that each subsector is composed of.
    // This is used to easily calculate which subsector has been clicked on the map.
    std::vector<seg_triangle> triangles;
};

struct seg_triangle {
    subsector* parent;
    std::array<vertex, 3> coords;
};

class doommap {
    display_config& _display_config;
    bool fully_loaded = false;
    std::vector<vertex> verts;
    std::vector<subsector> ssectors;
    user_interactions& _user_interactions;

    mutable std::mutex map_access_mutex;

  public:
    std::pair<int16_t, int16_t> min_vertex_coords = { INT16_MAX, INT16_MAX };
    std::pair<int16_t, int16_t> max_vertex_coords = { INT16_MIN, INT16_MIN };

  public:
    doommap(display_config& display_config, user_interactions& user_interactions);
    void clear();
    bool is_fully_loaded() const;
    void set_fully_loaded();
    void calculate_triangles();
    std::mutex& get_map_access_mutex() const;

    void add_vertex(vertex v);
    const std::vector<vertex>& get_verts() const;
    subsector& add_subsector();
    const std::vector<subsector>& get_ssectors() const;
};

} // namespace gienek
