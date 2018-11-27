#pragma once

#include <array>

#include "vertex.hpp"

namespace gienek {

class subsector;

class seg_triangle {
  public:
    subsector* parent;
    std::array<vertex, 3> coords;
};

} // namespace gienek
