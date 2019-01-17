#include "route_component.hpp"

namespace gienek {

route_component::route_component(int type)
    : _type(type)
    , _pt{ 0.0f, 0.0f } {};

route_component::route_component(int type, point<double> pt)
    : _type(type)
    , _pt(pt){};

} // namespace gienek
