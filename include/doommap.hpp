#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include <boost/format.hpp>

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

    bool operator==(const seg& rhs) const {
        return (sti == rhs.sti && eti == rhs.eti) || (eti == rhs.sti && sti == rhs.eti);
    }
};

extern std::map<int16_t, std::string> typename_to_id_map;

struct thing {
    uint16_t index;
    int16_t health;
    int16_t direction;
    int16_t posx;
    int16_t posy;
    int16_t posz;
    int16_t type;

  public:
    std::string get_tag() const { return (boost::format("(%1%, %2%)") % index % typename_to_id_map[type]).str(); }
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
    std::map<uint16_t, thing> things; // TODO: Should be map to allow trivial udpate_thing()
    user_interactions& _user_interactions;
    uint16_t player_thing_key{ 0 };

    mutable std::mutex map_access_mutex;

  public: // TODO: Make private
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
    const std::vector<std::int16_t> get_adjacent_subsectors(const subsector* ss) const;
    void add_thing(thing t);
    void update_thing(thing t);
    void remove_thing(uint16_t index);
    const std::map<uint16_t, thing>& get_things() const;
    void update_player_angle(int16_t angle);
};

} // namespace gienek
