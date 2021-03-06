#pragma once

namespace gienek {

enum CMD {
    UNKNOWN = 0,
    VERTEX = 'a',
    SUBSECTOR = 'b',
    THING = 'c',
    THING_UPDATE = 'd',
    THING_REMOVE = 'e',
    MAP_RECEIVED = 'f',
    PLAYER_ANGLE = 'g',
    SECTOR = 'h',
    LINE = 'i',
    SECTOR_CEILING_CHANGE = 'j',
    SECTOR_FLOOR_CHANGE = 'k',

    QUIT = 'q',
    CLEAR = 'x',
};

}
