#pragma once

#include "point.hpp"

namespace gienek {

class route_component {
  public:
    route_component(int type)
        : _type(type)
        , _pt{ 0.0f, 0.0f } {};
    route_component(int type, point<double> pt)
        : _type(type)
        , _pt(pt){};
    int _type; // 0 - point, 1 - use
    point<double> _pt;
};

} // namespace gienek
