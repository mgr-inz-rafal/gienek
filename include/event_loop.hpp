#pragma once

#include <allegro5/allegro.h>

#include "types.hpp"

namespace gienek {

class mouse;
class keyboard;
class painter;
class doommap;
class user_interactions;
class player;
class scaler;

class event_loop {
    ALLEGRO_EVENT _event;
    ALLEGRO_EVENT_QUEUE* _event_queue;

    player& _player;
    mouse& _mouse;
    keyboard& _keyboard;
    painter& _painter;
    doommap& _map;
    const gienek::scaler& _scaler;
    gienek::user_interactions& _user_interactions;

  public:
    event_loop(player& player, mouse& mouse, keyboard& keyboard, painter& painter, doommap& map,
                           user_interactions& user_interactions, ALLEGRO_EVENT_QUEUE* event_queue,
                           const scaler& scaler);
    void operator()(boost::asio::io_context& context, bool& quit);
};

} // namespace gienek
