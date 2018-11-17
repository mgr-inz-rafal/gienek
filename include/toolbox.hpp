#pragma once

#include "point.hpp"

#include <cstdint>
#include <utility>

namespace gienek {

class mouse;
class scaler;
class doommap;

struct toolbox {
    static bool point_in_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y);
    static std::pair<std::size_t, std::size_t> determine_clicked_triangle(mouse& mouse, scaler& scaler, doommap& map);
    static point<int16_t> window2map(point<int16_t> pt, const scaler& scaler);
};

} // namespace gienek
