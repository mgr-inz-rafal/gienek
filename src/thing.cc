#include "thing.hpp"
#include "toolbox.hpp"

#include <boost/format.hpp>

#include <map>

namespace gienek {

std::string thing::get_tag() const {
    return (boost::format("(%1%, %2%)") % index % toolbox::typename_to_id_map[type]).str();
}

} // namespace gienek
