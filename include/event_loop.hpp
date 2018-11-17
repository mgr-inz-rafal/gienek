#pragma once

#include <allegro5/allegro.h>

namespace gienek {

class mouse;
class keyboard;
class painter;
class doommap;
class user_interactions;
class player;

class event_loop {
    ALLEGRO_EVENT _event;
    ALLEGRO_EVENT_QUEUE* _event_queue;

    gienek::player& _player;
    gienek::mouse& _mouse;
    gienek::keyboard& _keyboard;
    gienek::painter& _painter;
    gienek::doommap& _map;
    gienek::user_interactions& _user_interactions;

  public:
    event_loop(gienek::player& player, gienek::mouse& mouse, gienek::keyboard& keyboard, gienek::painter& painter,
               gienek::doommap& map, gienek::user_interactions& user_interactions, ALLEGRO_EVENT_QUEUE* event_queue);
    void operator()(boost::asio::io_context& context, bool& quit);
};

} // namespace gienek
