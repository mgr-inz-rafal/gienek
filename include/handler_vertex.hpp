#pragma once

#include "handler.hpp"

namespace gienek {

class handler_vertex : public handler {
  protected:
    //    static std::size_t BUFFER_SIZE = 4;
    virtual handler_result handle() const override;
};

} // namespace gienek
