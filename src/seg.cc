#include "seg.hpp"

namespace gienek {

bool seg::operator==(const seg& rhs) const {
    return (sti == rhs.sti && eti == rhs.eti) || (eti == rhs.sti && sti == rhs.eti);
}

} // namespace gienek
