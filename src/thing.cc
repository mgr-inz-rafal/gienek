#include "thing.hpp"

#include <boost/format.hpp>

#include <map>

namespace gienek {

extern std::map<int16_t, std::string> typename_to_id_map;

std::string thing::get_tag() const {
    return (boost::format("(%1%, %2%)") % index % typename_to_id_map[type]).str();
}

} // namespace gienek
