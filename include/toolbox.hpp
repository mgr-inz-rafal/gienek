#pragma once

#include "point.hpp"

#include <cstdint>
#include <utility>

namespace gienek {

class mouse;
class scaler;
class doommap;
class scaler;

struct toolbox {
    static doommap* _map;
    static scaler* _scaler;
    static bool point_in_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y);
    static std::pair<std::size_t, std::size_t> position_to_triangle(mouse& mouse);
    static std::pair<std::size_t, std::size_t> position_to_triangle(point<int16_t> pos);
    static bool is_triangle_ok(const std::pair<std::size_t, std::size_t>& triangle);
    static point<int16_t> window2map(point<int16_t> pt);
};

} // namespace gienek
