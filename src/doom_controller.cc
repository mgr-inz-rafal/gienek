#include "doom_controller.hpp"

#include <chrono>
#include <thread>

namespace gienek {

void doom_controller::operator()(bool& quit) {
    using namespace std::chrono_literals;
    while (!quit) {
        std::this_thread::sleep_for(4ms);
    }

    int asd = 0;
    ++asd;
}

} // namespace gienek
