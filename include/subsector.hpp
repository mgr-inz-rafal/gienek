#pragma once

#include "point.hpp"
#include "seg.hpp"
#include "seg_triangle.hpp"

#include <cstdint>
#include <vector>

namespace gienek {

class subsector {
    const std::vector<vertex>& _verts;
    point<int16_t> _barycenter;
    int16_t _sector;

  public:
    subsector(const std::vector<vertex>& verts, int16_t sector);

    std::vector<seg> segs;

    // Calculate triangles that each subsector is composed of.
    // This is used to easily calculate which subsector has been clicked on the map.
    std::vector<seg_triangle> triangles;

    // This will calculate the point which can be treated as a "center" of convex polygon
    void calculate_barycenter();
    const point<int16_t>& get_barycenter() const;
    int16_t get_parent_sector() const;
};

} // namespace gienek
