#pragma once

#include "point.hpp"

#include <cstdint>
#include <map>
#include <utility>

namespace gienek {

class mouse;
class scaler;
class doommap;
class scaler;

struct toolbox {
    inline static std::map<int16_t, std::string> typename_to_id_map = {
        { 29999, "DoomPlayer" }, { 142, "CellPack" },  { 75, "Cell" },     { 134, "Berserk" },    { 28, "Chaingun" },
        { 30, "PlasmaRifle" },   { 51, "PlasmaBall" }, { 4, "Zombieman" }, { 131, "BulletPuff" }, { 11, "Clip" }
    };

    static doommap* _map;
    static scaler* _scaler;
    static bool point_in_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y);
    static std::pair<std::size_t, std::size_t> position_to_triangle(mouse& mouse);
    static std::pair<std::size_t, std::size_t> position_to_triangle(point<int16_t> pos);
    static bool is_triangle_ok(const std::pair<std::size_t, std::size_t>& triangle);
    static point<int16_t> window2map(point<int16_t> pt);
};

} // namespace gienek
