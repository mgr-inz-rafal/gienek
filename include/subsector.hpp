#pragma once

#include "seg.hpp"
#include "seg_triangle.hpp"

#include <vector>

namespace gienek {

class subsector {
  public:
    std::vector<seg> segs;

    // Calculate triangles that each subsector is composed of.
    // This is used to easily calculate which subsector has been clicked on the map.
    std::vector<seg_triangle> triangles;
};

} // namespace gienek
