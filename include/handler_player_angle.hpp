#pragma once

#include "handler.hpp"

namespace gienek {

class handler_player_angle : public handler {
  protected:
    virtual handler_result handle() const override;
    virtual size_t get_buffer_size() const override;
};

} // namespace gienek
