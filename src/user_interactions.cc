#include "user_interactions.hpp"

namespace gienek {

void user_interactions::reset() {
    _clicked_triangle = { std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max() };
}

void user_interactions::set_clicked_traingle(clicked_triangle_t clicked_triangle) {
    _clicked_triangle = std::move(clicked_triangle);
}

const clicked_triangle_t& user_interactions::get_clicked_triangle() const {
    return _clicked_triangle;
}

bool user_interactions::is_triangle_clicked() const {
    return std::numeric_limits<std::size_t>::max() != _clicked_triangle.first;
}

} // namespace gienek
