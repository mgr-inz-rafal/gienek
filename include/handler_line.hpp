#pragma once

#include "handler.hpp"

namespace gienek {

class handler_line : public handler {
  protected:
    std::size_t BUFFER_SIZE = 14;
    virtual handler_result handle() const override;
};

} // namespace gienek
