#pragma once

#include "point.hpp"

namespace gienek {

class mouse {
  public:
    point<double> mouse_click{ 0.0f, 0.0f };
};

} // namespace gienek
