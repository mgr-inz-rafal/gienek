#include "toolbox.hpp"
#include "doommap.hpp"
#include "line.hpp"
#include "mouse.hpp"
#include "scaler.hpp"

#include <boost/math/constants/constants.hpp>

namespace gienek {

doommap* toolbox::_map = nullptr;
scaler* toolbox::_scaler = nullptr;
std::array<int, 7> toolbox::doom_door_types = { 10, 11, 12, 13, 14, 202, 249 };
std::array<int, 13> toolbox::doom_platform_types = { 60, 61, 62, 63, 64, 65, 172, 203, 206, 207, 228, 230, 231 };
std::array<int, 1> toolbox::doom_teleport_types = { 70 };

bool toolbox::point_in_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y) {
    double denominator = (x1 * (y2 - y3) + y1 * (x3 - x2) + x2 * y3 - y2 * x3);
    double t1 = (x * (y3 - y1) + y * (x1 - x3) - x1 * y3 + y1 * x3) / denominator;
    double t2 = (x * (y2 - y1) + y * (x1 - x2) - x1 * y2 + y1 * x2) / -denominator;
    double s = t1 + t2;

    return 0 <= t1 && t1 <= 1 && 0 <= t2 && t2 <= 1 && s <= 1;
}

std::pair<std::size_t, std::size_t> toolbox::position_to_triangle(point<int16_t> pos) {
    const auto& ssectors = _map->get_ssectors();
    for (std::size_t i = 0; i < ssectors.size(); ++i) {
        for (std::size_t j = 0; j < ssectors[i].triangles.size(); ++j) {
            bool clicked = gienek::toolbox::point_in_triangle(
                ssectors[i].triangles[j].coords[0].x, ssectors[i].triangles[j].coords[0].y,
                ssectors[i].triangles[j].coords[1].x, ssectors[i].triangles[j].coords[1].y,
                ssectors[i].triangles[j].coords[2].x, ssectors[i].triangles[j].coords[2].y, pos.x, pos.y);
            if (clicked) {
                return { i, j };
            }
        }
    }
    return { std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max() };
}

bool toolbox::is_triangle_ok(const std::pair<std::size_t, std::size_t>& triangle) {
    return (triangle.first != std::numeric_limits<std::size_t>::max() &&
            triangle.second != std::numeric_limits<std::size_t>::max());
}

// TODO: Return std::optional
std::pair<std::size_t, std::size_t> toolbox::position_to_triangle(mouse& mouse) {
    auto mouse_position = window2map(mouse.mouse_click);
    return position_to_triangle({ mouse_position.x, mouse_position.y });
}

point<int16_t> toolbox::window2map(point<int16_t> pt) {
    const auto& cfg = _scaler->get_display_config();
    return { static_cast<int16_t>((pt.x - cfg.OFFSET_X) / cfg.SCALE),
             static_cast<int16_t>(((cfg.HEIGHT - pt.y) - cfg.OFFSET_Y) / cfg.SCALE) };
}

bool toolbox::are_angles_equal(double first, double second) {
    return std::abs(first - second) < angle_comparison_threshold;
}

bool toolbox::are_positions_equal(const point<double>& first, const point<double>& second) {
    return std::sqrt(((first.x - second.x) * (first.x - second.x)) + ((first.y - second.y) * (first.y - second.y))) <
           distance_comparison_threshold;
}

double toolbox::get_angle_between_points(const point<double>& first, const point<double>& second) {
    auto angle = (180.0f * std::atan2(second.y - first.y, second.x - first.x)) / boost::math::constants::pi<double>();
    while (angle < 0) {
        angle += 360.0f;
    }
    return std::fmod(angle, 360.0f);
}

player_turning_direction toolbox::get_player_turning_direction(double angle_player, double angle_target) {
    double distance_clockwise =
        (angle_player > angle_target) ? angle_player - angle_target : angle_player + 360.0f - angle_target;

    return (distance_clockwise < 180) ? player_turning_direction::RIGHT : player_turning_direction::LEFT;
}

bool toolbox::is_doom_door(const line* l) {
    if (nullptr == l) {
        return false;
    }

    return std::find(doom_door_types.begin(), doom_door_types.end(), l->type) != doom_door_types.end();
}

bool toolbox::is_doom_teleport(const line* l) {
    if (nullptr == l) {
        return false;
    }

    return std::find(doom_teleport_types.begin(), doom_teleport_types.end(), l->type) != doom_teleport_types.end();
}

bool toolbox::is_doom_platform(const line* l) {
    if (nullptr == l) {
        return false;
    }

    return std::find(doom_platform_types.begin(), doom_platform_types.end(), l->type) != doom_platform_types.end();
}

} // namespace gienek
