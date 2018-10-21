#pragma once

namespace gienek {

struct display_config {
    unsigned int WIDTH;
    unsigned int HEIGHT;
    double SCALE = -1.0f;
    double OFFSET_X = -1.0f;
    double OFFSET_Y = -1.0f;
};

} // namespace gienek
