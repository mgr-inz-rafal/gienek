#pragma once

#include "point.hpp"

namespace gienek {

class mouse {
  public:
    point<int16_t> mouse_click{ 0, 0 };
};

} // namespace gienek
