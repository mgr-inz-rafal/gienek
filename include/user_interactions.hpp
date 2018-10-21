#pragma once

#include <limits>
#include <utility>

namespace gienek {

using clicked_triangle_t = std::pair<std::size_t, std::size_t>;

class user_interactions {
    clicked_triangle_t _clicked_triangle = { std::numeric_limits<std::size_t>::max(),
                                             std::numeric_limits<std::size_t>::max() };

  public:
    void reset();
    void set_clicked_traingle(clicked_triangle_t clicked_triangle);
    bool is_triangle_clicked() const;
    const clicked_triangle_t& get_clicked_triangle() const;
};

} // namespace gienek
