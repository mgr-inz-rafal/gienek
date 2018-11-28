#pragma once

#include "point.hpp"
#include "seg.hpp"
#include "seg_triangle.hpp"

#include <vector>

namespace gienek {

class subsector {
    const std::vector<vertex>& _verts;
    point<int16_t> _barycenter;

  public:
    subsector(const std::vector<vertex>& verts);

    std::vector<seg> segs;

    // Calculate triangles that each subsector is composed of.
    // This is used to easily calculate which subsector has been clicked on the map.
    std::vector<seg_triangle> triangles;

    // This will calculate the point which can be treated as a "center" of convex polygon
    void calculate_barycenter();
    const point<int16_t>& get_barycenter() const;
};

} // namespace gienek
