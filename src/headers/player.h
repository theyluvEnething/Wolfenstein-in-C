#ifndef PLAYER_H
#define PLAYER_H

#include "vector2.h"

struct player {
    struct vector2 pos;
    double lookangle;
    double turnspeed;
    
    int fov;
    int raycount; // Responsible for resolution and precision

    int health;
};

#endif