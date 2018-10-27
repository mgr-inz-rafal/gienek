#pragma once

#include <allegro5/allegro.h>
#include <primitives/allegro5/allegro_primitives.h>

#include <algorithm>
#include <thread>

#include "display_config.hpp"
#include "doommap.hpp"
#include "mouse.hpp"
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
    scaler& _scaler;
    const user_interactions& _user_interactions;

    void draw_clicked_subsector(bool no_interior = false);
    void draw_clicked_triangle();
    void draw_vertexes();
    void draw_things();
    void calculate_display_adaptors();
    void draw_subsectors();
    void draw_subsector_interior(const subsector& ss, ALLEGRO_COLOR color);
    void draw_subsector_border(const subsector& ss, float width = 1.0f);
    void draw_mouse_pointer();

  public:
    painter(doommap& map, mouse& mouse, scaler& scaler, const user_interactions& user_interactions);

    void operator()(bool& quit);
    scaler& get_scaler();
};

} // namespace gienek
