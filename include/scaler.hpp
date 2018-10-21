#pragma once

#include "display_config.hpp"
#include "point.hpp"

namespace gienek {

class scaler {
    display_config& _display_config;

  public:
    scaler(display_config& display_config);
    point scale(const point& point) const;
    const display_config& get_display_config() const;
    void set_parameters(double scale, double offset_x, double offset_y);
};

} // namespace gienek
