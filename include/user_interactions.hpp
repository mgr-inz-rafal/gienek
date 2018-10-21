#pragma once

#include <limits>
#include <utility>

namespace gienek {

class user_interactions {
    std::pair<std::size_t, std::size_t> _clicked_triangle = { std::numeric_limits<std::size_t>::max(),
                                                              std::numeric_limits<std::size_t>::max() };

  public:
    void reset();
    void set_clicked_traingle(std::pair<std::size_t, std::size_t> clicked_triangle);
    bool is_triangle_clicked() const;
    const std::pair<std::size_t, std::size_t>& get_clicked_triangle() const;
};

} // namespace gienek
