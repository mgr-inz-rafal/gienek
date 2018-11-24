#include "player.hpp"
#include "doommap.hpp"
#include "player_states.hpp"
#include "toolbox.hpp"

namespace gienek {

class treenode {
  public:
    int16_t my_index;
    std::vector<treenode> leafs;
};

void path::generate_children(treenode& node) const {
    const auto& ss = _map->get_ssectors();
    const auto& subsector = ss[node.my_index];
    const auto& children = _map->get_adjacent_subsectors(&subsector);

    for (std::size_t i = 0; i < children.size(); ++i) {
        treenode new_node{ static_cast<int16_t>(children[i]) };
        new_node.leafs.emplace_back(new_node);
        generate_children(new_node);
    }
}

void path::calculate(point<int16_t> start, point<int16_t> end) {
    const auto& str = toolbox::position_to_triangle(start);
    const auto& etr = toolbox::position_to_triangle(end);

    treenode root{ static_cast<int16_t>(str.first) };
    generate_children(root);
}

player::player(gienek::doommap& map)
    : _map(map)
    , _state(player_states::IDLE)
    , _state_implementation(new IdlePlayerState) {
    _path.set_map(_map);
}

void player::set_state(player_states state) {
    _state = state;

    // TODO: Introduce factory
    switch (state) {
        case player_states::IDLE:
            _state_implementation = std::unique_ptr<BasePlayerState>(new IdlePlayerState);
            break;
        case player_states::MOVING_TO:
            _state_implementation = std::unique_ptr<BasePlayerState>(new MovingToPlayerState);
            break;
    }
};

void player::go_to(point<int16_t> target) {
    _target = target;
    set_state(player_states::MOVING_TO);
}

std::optional<point<int16_t>> player::get_target() const {
    return _target;
}

const BasePlayerState& player::get_state() const {
    return *_state_implementation;
}

void player::operator()() {
    for (;;) {
        switch (_state) {
            case player_states::IDLE:
                break;
            case player_states::MOVING_TO:
                if (!_path.calculated) {
                    _path._route.clear();
                    _path.calculate(_player.pos, _target.value());
                }
                break;
        }
    }
}

actor& player::get_actor() {
    return _player;
}

} // namespace gienek
