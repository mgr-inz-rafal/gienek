#pragma once

#include "handler.hpp"

namespace gienek {

class handler_thing_update : public handler {
  protected:
    virtual handler_result handle() const override;
};

} // namespace gienek
