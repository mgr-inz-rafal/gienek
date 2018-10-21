#include "toolbox.hpp"
#include "doommap.hpp"
#include "mouse.hpp"
#include "scaler.hpp"

namespace gienek {

bool toolbox::point_in_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y) {
    double denominator = (x1 * (y2 - y3) + y1 * (x3 - x2) + x2 * y3 - y2 * x3);
    double t1 = (x * (y3 - y1) + y * (x1 - x3) - x1 * y3 + y1 * x3) / denominator;
    double t2 = (x * (y2 - y1) + y * (x1 - x2) - x1 * y2 + y1 * x2) / -denominator;
    double s = t1 + t2;

    return 0 <= t1 && t1 <= 1 && 0 <= t2 && t2 <= 1 && s <= 1;
}

std::pair<std::size_t, std::size_t> toolbox::determine_clicked_triangle(mouse& mouse, scaler& scaler, doommap& map) {
    const auto& ssectors = map.get_ssectors();
    const auto& cfg = scaler.get_display_config();
    for (std::size_t i = 0; i < ssectors.size(); ++i) {
        for (std::size_t j = 0; j < ssectors[i].triangles.size(); ++j) {
            auto x = (mouse.mouse_click.x - cfg.OFFSET_X) / cfg.SCALE;
            auto y = ((cfg.HEIGHT - mouse.mouse_click.y) - cfg.OFFSET_Y) / cfg.SCALE;
            bool clicked = gienek::toolbox::point_in_triangle(
                ssectors[i].triangles[j].coords[0].x, ssectors[i].triangles[j].coords[0].y,
                ssectors[i].triangles[j].coords[1].x, ssectors[i].triangles[j].coords[1].y,
                ssectors[i].triangles[j].coords[2].x, ssectors[i].triangles[j].coords[2].y, x, y);
            if (clicked) {
                return { i, j };
            }
        }
    }
    return { std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max() };
}

} // namespace gienek
