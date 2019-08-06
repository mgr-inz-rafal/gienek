#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include <algorithm>
#include <chrono>
#include <optional>
#include <thread>

#include "display_config.hpp"
#include "doommap.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "player.hpp"
#include "point.hpp"
#include "scaler.hpp"
#include "toolbox.hpp"
#include "user_interactions.hpp"

namespace gienek {

using click_info_t = std::optional<std::pair<int16_t, int16_t>>;

class painter {
  private:
    // TODO: Make consts
    ALLEGRO_DISPLAY* display = NULL;
    doommap& _map; // TODO: bring back const (dupa)
    mouse& _mouse;
    keyboard& _keyboard;
    scaler& _scaler;
    player& _player;
    const user_interactions& _user_interactions;
    ALLEGRO_FONT* font = NULL;
    point<double> item_tag_offset = { 5, 5 };
    std::string pressed_keys;
    void draw_point(const std::optional<point<int16_t>>& pt, ALLEGRO_COLOR color);
    std::string point_to_string(const std::optional<point<int16_t>>& pt) const;
    click_info_t get_clicked_sector_subsector_indices() const;

    // Extract to "path_painter" class or smth
    std::chrono::time_point<std::chrono::system_clock> _last_path_color_switch;
    double _path_color_offset = 0.0f;

    void draw_clicked_subsector(bool no_interior = false);
    void draw_clicked_triangle();
    void draw_vertexes();
    void draw_special_lines();
    void draw_things();
    void calculate_display_adaptors();
    void draw_subsectors();
    void draw_subsector_interior(const subsector& ss, ALLEGRO_COLOR color);
    void draw_subsector_border(const subsector& ss, float width = 1.0f);
    void draw_mouse_pointer();
    void draw_pressed_keys();
    void draw_clicked_subsector_info();
    void draw_clicked_sector_info();
    void draw_player_target();
    void draw_path();
    void draw_player_status_text() const;

    double thing_angle_to_radians(int16_t direction);

  public:
    painter(doommap& map, player& player, mouse& mouse, keyboard& keyboard, scaler& scaler,
            const user_interactions& user_interactions);

    void operator()(bool& quit, ALLEGRO_EVENT_QUEUE* event_queue);
    scaler& get_scaler();
};

} // namespace gienek
