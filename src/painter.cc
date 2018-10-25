#include "painter.hpp"

namespace gienek {

painter::painter(doommap& map, mouse& mouse, scaler& scaler, const user_interactions& user_interactions)
    : _map(map)
    , _mouse(mouse)
    , _scaler(scaler)
    , _user_interactions(user_interactions){};

void painter::operator()(bool& quit) {
    ALLEGRO_DISPLAY* display = NULL;
    display = al_create_display(_scaler.get_display_config().WIDTH, _scaler.get_display_config().HEIGHT);
    if (!display) {
        // return -1;
    }
    al_set_window_title(display, "Gienek AssAssYn");
    while (!quit) {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (_map.is_fully_loaded()) {
            std::lock_guard guard(_map.get_map_access_mutex());
            if (_scaler.get_display_config().SCALE < 0.0f) {
                calculate_display_adaptors();
            }

            // draw_vertexes();
            draw_subsectors();
            draw_clicked_subsector();
            draw_clicked_triangle();
            draw_mouse_pointer();
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60)); // ~60FPS

        al_flip_display();
    }
    al_destroy_display(display);
}

void painter::calculate_display_adaptors() {
    const auto& verts = _map.get_verts();
    if (verts.size() < 2) {
        return;
    }
    std::vector<int16_t> xs, ys;
    for (const auto& v : verts) {
        xs.push_back(v.x);
        ys.push_back(v.y);
    }
    double horizontal_span = _map.max_vertex_coords.first - _map.min_vertex_coords.first;
    double horizontal_scale = _scaler.get_display_config().WIDTH / horizontal_span;

    double vertical_span = _map.max_vertex_coords.second - _map.min_vertex_coords.second;
    double vertical_scale = _scaler.get_display_config().HEIGHT / vertical_span;

    double scale = std::min(horizontal_scale, vertical_scale);
    _scaler.set_parameters(scale, -_map.min_vertex_coords.first * scale, -_map.min_vertex_coords.second * scale);
}

void painter::draw_vertexes() {
    for (const auto& v : _map.get_verts()) {
        point p = _scaler.scale({ static_cast<double>(v.x), static_cast<double>(v.y) });
        al_put_pixel(p.x, p.y, al_map_rgb(255, 255, 255));
    }
}

void painter::draw_clicked_triangle() {
    if (!_user_interactions.is_triangle_clicked()) {
        return;
    }
    const clicked_triangle_t& clicked_triangle = _user_interactions.get_clicked_triangle();

    const auto& ssectors = _map.get_ssectors();
    const auto& coord1 = ssectors[clicked_triangle.first].triangles[clicked_triangle.second].coords[0];
    point pt1 = _scaler.scale({ static_cast<double>(coord1.x), static_cast<double>(coord1.y) });

    const auto& coord2 = ssectors[clicked_triangle.first].triangles[clicked_triangle.second].coords[1];
    point pt2 = _scaler.scale({ static_cast<double>(coord2.x), static_cast<double>(coord2.y) });

    const auto& coord3 = ssectors[clicked_triangle.first].triangles[clicked_triangle.second].coords[2];
    point pt3 = _scaler.scale({ static_cast<double>(coord3.x), static_cast<double>(coord3.y) });

    al_draw_filled_triangle(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, al_map_rgb(0, 64 + 8, 0));
}

void painter::draw_clicked_subsector() {
    if (!_user_interactions.is_triangle_clicked()) {
        return;
    }
    const clicked_triangle_t& clicked_triangle = _user_interactions.get_clicked_triangle();
    const auto& ssectors = _map.get_ssectors();
    const auto& triangle = ssectors[clicked_triangle.first].triangles[clicked_triangle.second];
    draw_subsector_interior(*triangle.parent, al_map_rgb(0, 64, 0));
    draw_subsector_border(*triangle.parent);
}

void painter::draw_subsector_interior(const subsector& ss, ALLEGRO_COLOR color) {
    for (const auto& triangle : ss.triangles) {
        point pt1 =
            _scaler.scale({ static_cast<double>(triangle.coords[0].x), static_cast<double>(triangle.coords[0].y) });
        point pt2 =
            _scaler.scale({ static_cast<double>(triangle.coords[1].x), static_cast<double>(triangle.coords[1].y) });
        point pt3 =
            _scaler.scale({ static_cast<double>(triangle.coords[2].x), static_cast<double>(triangle.coords[2].y) });
        al_draw_filled_triangle(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, color);
    }
}

void painter::draw_subsector_border(const subsector& ss) {
    for (const auto& seg : ss.segs) {
        const auto& verts = _map.get_verts();
        auto v1 = verts[seg.sti];
        auto v2 = verts[seg.eti];
        point pt1 = _scaler.scale({ static_cast<double>(v1.x), static_cast<double>(v1.y) });
        point pt2 = _scaler.scale({ static_cast<double>(v2.x), static_cast<double>(v2.y) });
        al_draw_line(pt1.x, pt1.y, pt2.x, pt2.y, al_map_rgb(255, 255, 0), 1.0f);
    }
}

void painter::draw_subsectors() {
    for (const auto& subsector : _map.get_ssectors()) {
        draw_subsector_interior(subsector, al_map_rgb(0, 32, 0));
        draw_subsector_border(subsector);
    }
}

void painter::draw_mouse_pointer() {
    al_draw_filled_circle(_mouse.mouse_click.x, _mouse.mouse_click.y, 3.0f, al_map_rgb(255, 255, 255));
}

scaler& painter::get_scaler() {
    return _scaler;
}

} // namespace gienek
