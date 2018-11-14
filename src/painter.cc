#include "painter.hpp"

#include <boost/math/constants/constants.hpp>
#include <chrono>

namespace gienek {

painter::painter(doommap& map, mouse& mouse, scaler& scaler, const user_interactions& user_interactions)
    : _map(map)
    , _mouse(mouse)
    , _scaler(scaler)
    , _user_interactions(user_interactions) {
    font = al_load_bitmap_font("d:\\Git\\gienek\\fonts\\a4_font.tga");
};

void painter::operator()(bool& quit, ALLEGRO_EVENT_QUEUE* event_queue) {
    using namespace std::chrono_literals;
    ALLEGRO_DISPLAY* display = NULL;
    display = al_create_display(_scaler.get_display_config().WIDTH, _scaler.get_display_config().HEIGHT);
    if (!display) {
        // return -1;
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));
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
            draw_clicked_subsector(true);
            draw_things();
            draw_mouse_pointer();

            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Map loaded");
        } else {
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Gienek is waiting for map to be received");
        }

        al_flip_display();
        std::this_thread::sleep_for(4ms);
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

void painter::draw_things() {
    for (const auto& t : _map.get_things()) {
        point p = _scaler.scale({ static_cast<double>(t.second.posx), static_cast<double>(t.second.posy) });

        al_draw_text(font, al_map_rgb(255, 255, 255), p.x + item_tag_offset.x, p.y + item_tag_offset.y, 0,
                     t.second.get_tag().c_str());

        ALLEGRO_TRANSFORM tr;
        al_identity_transform(&tr);
        al_translate_transform(&tr, -p.x, -p.y);
        al_rotate_transform(&tr, thing_angle_to_radians(t.second.direction));
        al_translate_transform(&tr, p.x, p.y);
        al_use_transform(&tr);

        al_draw_triangle(p.x - 6, p.y - 4, p.x + 6, p.y, p.x - 6, p.y + 4, al_map_rgb(255, 255, 255), 1.0f);

        al_identity_transform(&tr);
        al_use_transform(&tr);
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

    al_draw_filled_triangle(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, al_map_rgb(96, 0, 0));
}

void painter::draw_clicked_subsector(bool no_interior) {
    if (!_user_interactions.is_triangle_clicked()) {
        return;
    }
    const clicked_triangle_t& clicked_triangle = _user_interactions.get_clicked_triangle();
    const auto& ssectors = _map.get_ssectors();
    const auto& triangle = ssectors[clicked_triangle.first].triangles[clicked_triangle.second];
    if (!no_interior) {
        draw_subsector_interior(*triangle.parent, al_map_rgb(80, 0, 0));
    }
    draw_subsector_border(*triangle.parent, 3.5f);

    const auto& adjacent = _map.get_adjacent_subsectors(triangle.parent);
    for (const auto& ss : adjacent) {
        draw_subsector_interior(ssectors[ss], al_map_rgb(64, 0, 0));
        draw_subsector_border(ssectors[ss], 3.5f);
    }
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

void painter::draw_subsector_border(const subsector& ss, float width) {
    for (const auto& seg : ss.segs) {
        const auto& verts = _map.get_verts();
        auto v1 = verts[seg.sti];
        auto v2 = verts[seg.eti];
        point pt1 = _scaler.scale({ static_cast<double>(v1.x), static_cast<double>(v1.y) });
        point pt2 = _scaler.scale({ static_cast<double>(v2.x), static_cast<double>(v2.y) });
        al_draw_line(pt1.x, pt1.y, pt2.x, pt2.y, al_map_rgb(255, 255, 0), width);
    }
}

void painter::draw_subsectors() {
    const auto& subsectors = _map.get_ssectors();
    for (std::size_t i = 0; i < subsectors.size(); ++i) {
        draw_subsector_interior(subsectors[i], al_map_rgb(0, 32, 0));
        draw_subsector_border(subsectors[i]);
    }
}

void painter::draw_mouse_pointer() {
    al_draw_filled_circle(_mouse.mouse_click.x, _mouse.mouse_click.y, 3.0f, al_map_rgb(255, 255, 255));
}

scaler& painter::get_scaler() {
    return _scaler;
}

double painter::thing_angle_to_radians(int16_t direction) {
    return (static_cast<double>(-direction) * boost::math::constants::pi<double>()) / 180;
}

} // namespace gienek
