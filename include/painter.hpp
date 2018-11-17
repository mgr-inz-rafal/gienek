#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include <algorithm>
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

class painter {
  private:
    // TODO: Make consts
    ALLEGRO_DISPLAY* display = NULL;
    const doommap& _map;
    mouse& _mouse;
    keyboard& _keyboard;
    scaler& _scaler;
    const player& _player;
    const user_interactions& _user_interactions;
    ALLEGRO_FONT* font = NULL;
    point<double> item_tag_offset = { 5, 5 };
    std::string pressed_keys;

    void draw_clicked_subsector(bool no_interior = false);
    void draw_clicked_triangle();
    void draw_vertexes();
    void draw_things();
    void calculate_display_adaptors();
    void draw_subsectors();
    void draw_subsector_interior(const subsector& ss, ALLEGRO_COLOR color);
    void draw_subsector_border(const subsector& ss, float width = 1.0f);
    void draw_mouse_pointer();
    void draw_pressed_keys();
    void draw_player_target();
    double thing_angle_to_radians(int16_t direction);

  public:
    painter(doommap& map, const player& player, mouse& mouse, keyboard& keyboard, scaler& scaler,
            const user_interactions& user_interactions);

    void operator()(bool& quit, ALLEGRO_EVENT_QUEUE* event_queue);
    scaler& get_scaler();
};

} // namespace gienek
