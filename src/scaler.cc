#include "scaler.hpp"

namespace gienek {

scaler::scaler(display_config& display_config)
    : _display_config(display_config) {}

point scaler::scale(const point& point) const {
    return { point.x * _display_config.SCALE + _display_config.OFFSET_X,
             _display_config.HEIGHT - (point.y * _display_config.SCALE + _display_config.OFFSET_Y) };
}

const display_config& scaler::get_display_config() const {
    return _display_config;
}

void scaler::set_parameters(double scale, double offset_x, double offset_y) {
    _display_config.SCALE = scale;
    _display_config.OFFSET_X = offset_x;
    _display_config.OFFSET_Y = offset_y;
}

} // namespace gienek
