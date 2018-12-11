#include "player_task.hpp"

namespace gienek {

player_task::player_task()
    : _current{ type::STAND_STILL } {}

player_task::type player_task::get_current_task() const {
    return _current;
}

} // namespace gienek
