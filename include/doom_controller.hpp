#pragma once

#include <string>

namespace gienek {

class doom_controller {
    std::string _address;

  public:
    doom_controller(const std::string& address);
    void operator()(bool& quit);
};

} // namespace gienek
