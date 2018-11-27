#pragma once

namespace gienek {

template <typename T>
struct point {
    T x;
    T y;

    bool operator==(const point<T>& rhs) const { return x == rhs.x && y == rhs.y; };
};

} // namespace gienek
