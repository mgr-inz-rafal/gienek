#pragma once

#include "handler.hpp"

namespace gienek {

class player;

class handler_clear : public handler {
    player& _player;

  protected:
    virtual handler_result handle() const override;

  public:
    handler_clear(player& player);
};

} // namespace gienek
