#ifndef PLAYER_H
#define PLAYER_H

#include "vector2.h"

struct player {
    struct vector2 pos;
    double angle;
    int health;
};

#endif