#include "toolbox.hpp"
#include "doommap.hpp"
#include "mouse.hpp"
#include "scaler.hpp"

namespace gienek {

doommap* toolbox::_map = nullptr;
scaler* toolbox::_scaler = nullptr;

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

} // namespace gienek
