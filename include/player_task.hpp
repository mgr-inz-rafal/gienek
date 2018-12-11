#pragma once

namespace gienek {

class player_task {
  public:
    enum class type { STAND_STILL };

  private:
    type _current;

  public:
    player_task();
    player_task::type get_current_task() const;
};

} // namespace gienek
