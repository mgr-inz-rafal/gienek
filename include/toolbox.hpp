#pragma once

#include "point.hpp"

#include <array>
#include <cstdint>
#include <map>
#include <utility>

namespace gienek {

class mouse;
class scaler;
class doommap;
class scaler;
class line;

enum class player_turning_direction { LEFT, RIGHT };

struct toolbox {
    static std::array<int, 7> doom_door_types;
    static std::array<int, 13> doom_platform_types;

    // This must be fairly large to compensate the rotation that
    // player will be able to do before receiving the stop signal.
    const inline static double angle_comparison_threshold{ 2.0f };
    const inline static double distance_comparison_threshold{ 68.0f };

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
    static bool are_angles_equal(double first, double second);
    static bool are_positions_equal(const point<double>& first, const point<double>& second);
    static double get_angle_between_points(const point<double>& first, const point<double>& second);
    static player_turning_direction get_player_turning_direction(double angle_player, double angle_target);
    static bool is_doom_door(const line* l);
    static bool is_doom_platform(const line* l);
};

} // namespace gienek
