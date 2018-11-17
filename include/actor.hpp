#pragma once

#include "actor_actions.hpp"
#include "point.hpp"

#include <map>

namespace gienek {
class actor {
  public:
    gienek::point<int16_t> pos;
    gienek::point<int16_t> target;
    std::map<actor_actions, bool> actions;

    void set_action(actor_actions act, bool onoff);
};
} // namespace gienek
