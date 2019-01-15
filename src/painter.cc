#include "painter.hpp"
#include "doommap.hpp"
#include "path.hpp"
#include "player.hpp"
#include "toolbox.hpp"

#include <boost/format.hpp>
#include <boost/math/constants/constants.hpp>
#include <chrono>
#include <optional>

namespace gienek {

painter::painter(doommap& map, player& player, mouse& mouse, keyboard& keyboard, scaler& scaler,
                 const user_interactions& user_interactions)
    : _map(map)
    , _player(player)
    , _mouse(mouse)
    , _keyboard(keyboard)
    , _scaler(scaler)
    , _user_interactions(user_interactions)
    , _last_path_color_switch(std::chrono::system_clock::now()) {
    font = al_load_bitmap_font("a4_font.tga");
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
            draw_special_lines();
            draw_path();
            draw_things();
            draw_mouse_pointer();
            draw_player_target();

            // Console info
            draw_clicked_subsector_info();
            draw_clicked_sector_info();
            draw_player_status_text();
            draw_pressed_keys();

            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Map loaded");
        } else {
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Gienek is waiting for map to be received");
        }

        al_flip_display();
        std::this_thread::sleep_for(4ms);
    }
    al_destroy_display(display);
}

void painter::draw_pressed_keys() {
    pressed_keys = "Keys: ";
    for (const auto& key : _keyboard.keystate) {
        if (key.second) {
            pressed_keys += al_keycode_to_name(key.first);
            pressed_keys += ' ';
        }
    }
    al_draw_text(font, al_map_rgb(255, 255, 255), 256, 16, 0, pressed_keys.c_str());
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

void painter::draw_special_lines() {
    const auto& lines = _map.get_lines();
    for (const auto& l : lines) {
        if (l.type == 0) {
            continue;
        }

        point pt1 = _scaler.scale({ static_cast<double>(l.x1), static_cast<double>(l.y1) });
        point pt2 = _scaler.scale({ static_cast<double>(l.x2), static_cast<double>(l.y2) });
        al_draw_line(pt1.x, pt1.y, pt2.x, pt2.y, al_map_rgb(0, 255, 255), 7.0f);
    }
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

    const auto& barycenter = triangle.parent->get_barycenter();
    point scaled = _scaler.scale({ static_cast<double>(barycenter.x), static_cast<double>(barycenter.y) });
    al_draw_filled_rectangle(scaled.x - 3, scaled.y - 3, scaled.x + 3, scaled.y + 3, al_map_rgb(80, 80, 0));
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

click_info_t painter::get_clicked_sector_subsector_indices() const {
    if (!_user_interactions.is_triangle_clicked()) {
        return std::nullopt;
    }
    const auto& ssectors = _map.get_ssectors();
    const clicked_triangle_t& clicked_triangle = _user_interactions.get_clicked_triangle();
    int16_t ssector = static_cast<int16_t>(clicked_triangle.first);
    int16_t sector = ssectors[ssector].get_parent_sector();
    return { std::pair{ ssector, sector } };
}

void painter::draw_clicked_sector_info() {
    const auto secinfo = get_clicked_sector_subsector_indices();
    if (secinfo.has_value()) {
        const auto& sectors = _map.get_sectors();
        const auto& sector = sectors[secinfo->second];
        al_draw_text(font, al_map_rgb(255, 255, 255), 0, 48, 0,
                     (boost::format("  Ceiling: %1%") % sector.get_ceiling_height()).str().c_str());
        al_draw_text(font, al_map_rgb(255, 255, 255), 0, 48 + 16, 0,
                     (boost::format("    Floor: %1%") % sector.get_floor_height()).str().c_str());
        al_draw_text(font, al_map_rgb(255, 255, 255), 0, 48 + 32, 0,
                     (boost::format("      Tag: %1%") % sector.get_tag()).str().c_str());
    }
}

void painter::draw_clicked_subsector_info() {
    const auto secinfo = get_clicked_sector_subsector_indices();
    if (secinfo.has_value()) {
        al_draw_text(font, al_map_rgb(255, 255, 255), 0, 16, 0,
                     (boost::format("Sector/Subs.: %1%/%2%") % secinfo->first % secinfo->second).str().c_str());
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

void painter::draw_path() {
    using namespace std::chrono_literals;

    path& path = _player.get_path();
    std::lock_guard lock(path.get_path_calculation_mutex());
    if (!path.calculated) {
        return;
    }
    const auto& subsectors = _map.get_ssectors();
    if (subsectors.empty()) {
        return;
    }
    const auto& route = path.get_route_subsectors();
    if (route.empty()) {
        return;
    }

    unsigned char start_color = 128;
    unsigned char end_color = 255;
    double increment = static_cast<double>(end_color - start_color) / route.size();

    std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = current - _last_path_color_switch;
    if (diff > 20ms) {
        _last_path_color_switch = current;
        _path_color_offset += increment;
    }

    // Draw flashy subsectors
    for (const auto& element : route) {
        double color = end_color + _path_color_offset;
        if (color > 256) {
            color = start_color + (color - 256);
        }
        color = 256 - color;
        draw_subsector_interior(subsectors[element], al_map_rgb(0, 0, color));
        draw_subsector_border(subsectors[element]);
        end_color -= increment;
    }

    // Draw barycenter line on top
    auto route_points = path.get_route_points();
    if (route_points.empty()) {
        return;
    }
    auto pt = route_points.cbegin();
    for (;;) {
        auto first = *pt;
        ++pt;
        if (route_points.cend() == pt) {
            break;
        }
        auto second = *pt;

        point<double> pt1 = _scaler.scale(first);
        point<double> pt2 = _scaler.scale(second);
        al_draw_line(pt1.x, pt1.y, pt2.x, pt2.y, al_map_rgb(255, 255, 255), 1.0f);
    }
}

void painter::draw_mouse_pointer() {
    al_draw_filled_circle(_mouse.mouse_click.x, _mouse.mouse_click.y, 3.0f, al_map_rgb(255, 255, 255));
    point<int16_t> scaled = toolbox::window2map({ _mouse.mouse_click.x, _mouse.mouse_click.y });
    al_draw_text(font, al_map_rgb(255, 255, 255), _mouse.mouse_click.x + 5, _mouse.mouse_click.y + 5, 0,
                 (boost::format("(%1%,%2%)") % scaled.x % scaled.y).str().c_str());
}

scaler& painter::get_scaler() {
    return _scaler;
}

double painter::thing_angle_to_radians(int16_t direction) {
    return (static_cast<double>(-direction) * boost::math::constants::pi<double>()) / 180;
}

void painter::draw_point(const std::optional<point<int16_t>>& pt, ALLEGRO_COLOR color) {
    if (pt.has_value()) {
        auto point = _scaler.scale({ static_cast<double>(pt->x), static_cast<double>(pt->y) });
        al_draw_filled_circle(point.x, point.y, 3.0f, color);
    }
}

void painter::draw_player_target() {
    draw_point(_player.get_target(), al_map_rgb(255, 0, 0));

    auto next_step = _player.get_next_route_point();
    if (next_step.has_value()) {
        auto next_step_scaled = _scaler.scale({ static_cast<double>(next_step->x), static_cast<double>(next_step->y) });
        al_draw_filled_circle(next_step_scaled.x, next_step_scaled.y, 10.0f, al_map_rgb(255, 255, 255));
    }
}

std::string painter::point_to_string(const std::optional<point<int16_t>>& pt) const {
    if (pt.has_value()) {
        return (boost::format("(%1%,%2%)") % pt.value().x % pt.value().y).str();
    }
    return "unspecified";
}

void painter::draw_player_status_text() const {
    std::string target_string = point_to_string(_player.get_target());
    std::string next_point_string = point_to_string(_player.get_next_route_point());
    std::string status =
        (boost::format("State: %1%, Target=%2%, Next=%3%, Angle=%4%/%5%") % _player.get_state().to_string() %
         target_string % next_point_string % _player.get_actor().angle % _player.get_angle_to_next_target_point())
            .str();

    al_draw_text(font, al_map_rgb(255, 255, 255), 0, 32, 0, status.c_str());
}

} // namespace gienek
