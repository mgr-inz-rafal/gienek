#pragma once

#include "point.hpp"

namespace gienek {

class route_component {
  public:
    route_component(int type);
    route_component(int type, point<double> pt);
    int _type; // 0 - point, 1 - use
    point<double> _pt;
};

} // namespace gienek
