#pragma once

#include <cstdint>

namespace gienek {

class seg {
  public:
    // Start and end vertex index
    int16_t sti;
    int16_t eti;
    int16_t linei;

    bool operator==(const seg& rhs) const;
};

} // namespace gienek
