#include "subsector.hpp"
#include "point.hpp"

#include <algorithm>
#include <limits>
#include <unordered_set>
#include <vector>

namespace gienek {

subsector::subsector(const std::vector<vertex>& verts)
    : _verts(verts) {}

const point<int16_t>& subsector::get_barycenter() const {
    return _barycenter;
}

void subsector::calculate_barycenter() {
    auto hasher = [](point<int16_t> const& xxx) { return xxx.x * std::numeric_limits<int16_t>::max() + xxx.y; };
    std::unordered_set<point<int16_t>, decltype(hasher)> points(0, hasher);

    for (unsigned short i = 0; i < segs.size(); ++i) {
        auto start_vertex = _verts[segs[i].sti];
        auto end_vertex = _verts[segs[i].eti];

        point<int16_t> start_point{ start_vertex.x, start_vertex.y };
        point<int16_t> end_point{ end_vertex.x, end_vertex.y };

        points.emplace(start_point);
        points.emplace(end_point);
    }

    double SX = 0, SY = 0, SL = 0;
    auto& it = points.cbegin();
    bool done = false;
    while (!done) {
        const auto& p1 = *it;
        ++it;
        if (it == points.end()) {
            it = points.begin();
            done = true;
        }
        const auto& p2 = *it;

        double L = ((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)) / 2;
        SX += (p1.x + p2.x) / 2 * L;
        SY += (p1.y + p2.y) / 2 * L;
        SL += L;
    }

    _barycenter.x = static_cast<int16_t>(SX / SL);
    _barycenter.y = static_cast<int16_t>(SY / SL);
}

} // namespace gienek
