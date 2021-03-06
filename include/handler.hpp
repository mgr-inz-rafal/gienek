#pragma once

#include "cmd_types.hpp"
#include "doommap.hpp"
#include "handler_result.hpp"
#include "player.hpp"
#include "socket_reader.hpp"

#include <cstdint>
#include <string>

namespace gienek {

class handler {
  protected:
    socket_reader* _reader;
    doommap* _map;
    player* _player;

  public:
    virtual handler_result handle() const = 0;
    virtual size_t get_buffer_size() const;
    virtual void set_socket_reader(socket_reader& reader);
    virtual void set_doommap(doommap& doommap);
    virtual void set_player(player& player);
    virtual ~handler() = default;
};

} // namespace gienek
