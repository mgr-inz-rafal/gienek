#include "subsector.hpp"
#include "point.hpp"

#include <algorithm>
#include <vector>

namespace gienek {

subsector::subsector(const std::vector<vertex>& verts)
    : _verts(verts) {}

void subsector::insert_point_if_not_existing(const point<int16_t>& pt) {
    if (barycenter_points.end() == std::find(barycenter_points.begin(), barycenter_points.end(), pt)) {
        barycenter_points.push_back(pt);
    }
}

const point<int16_t>& subsector::get_barycenter_point_at(int index) const {
    return index == -1 ? barycenter_points[barycenter_points.size() - 1] : barycenter_points[index];
}

const point<int16_t>& subsector::get_barycenter() const {
    return _barycenter;
}

void subsector::calculate_barycenter() {
    barycenter_points.clear();
    for (unsigned short i = 0; i < segs.size(); ++i) {
        auto start_vertex = _verts[segs[i].sti];
        auto end_vertex = _verts[segs[i].eti];

        point<int16_t> start_point{ start_vertex.x, start_vertex.y };
        point<int16_t> end_point{ end_vertex.x, end_vertex.y };

        insert_point_if_not_existing(start_point);
        insert_point_if_not_existing(end_point);
    }

    double SX = 0, SY = 0, SL = 0;
    for (size_t i = 0; i < barycenter_points.size(); ++i) {
        const auto& p1 = get_barycenter_point_at(i - 1);
        const auto& p2 = get_barycenter_point_at(i);
        double L = ((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)) / 2;
        SX += (p1.x + p2.x) / 2 * L;
        SY += (p1.y + p2.y) / 2 * L;
        SL += L;
    }
    _barycenter.x = static_cast<int16_t>(SX / SL);
    _barycenter.y = static_cast<int16_t>(SY / SL);
}

} // namespace gienek
