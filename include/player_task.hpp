#pragma once

namespace gienek {

class player_task {
  public:
    enum class type { STAND_STILL, TRANSFER_TO_TARGET };

  private:
    type _current;

  public:
    player_task();
    player_task::type get_current_task() const;
};

} // namespace gienek
