#pragma once

namespace gienek {

enum CMD {
    UNKNOWN = 0,
    VERTEX = 'a',
    QUIT = 'q',
    SUBSECTOR = 'b',
    CLEAR = 'x',
    MAP_RECEIVED = 'f',
    THING = 'c',
    THING_UPDATE = 'd',
    THING_REMOVE = 'e'
};

}
