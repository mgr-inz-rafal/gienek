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

extern std::pair<std::size_t, std::size_t> clicked_triangle;

namespace gienek {

class painter {
  private:
    // TODO: Make consts
    ALLEGRO_DISPLAY* display = NULL;
    const doommap& _map;
    mouse& _mouse;
    scaler& _scaler;

    void draw_clicked_subsector();
    void draw_clicked_triangle();
    void draw_vertexes();
    void calculate_display_adaptors();
    void draw_subsectors();
    void draw_subsector_interior(const subsector& ss, ALLEGRO_COLOR color);
    void draw_subsector_border(const subsector& ss);
    void draw_mouse_pointer();

  public:
    painter(doommap& map, mouse& mouse, scaler& scaler)
        : _map(map)
        , _mouse(mouse)
        , _scaler(scaler){};

    void operator()(bool& quit);
    scaler& get_scaler();
};

} // namespace gienek
