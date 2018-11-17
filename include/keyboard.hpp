#pragma once

#include <allegro5/allegro.h>
#include <map>

namespace gienek {

class keyboard {
  public:
    std::map<int, bool> keystate;
};

} // namespace gienek
