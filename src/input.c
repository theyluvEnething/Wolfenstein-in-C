#include <stdio.h>
#include <math.h>

#include "headers/input.h"
#include "headers/const.h"
#include "headers/func.h"

void HandleKeyboardInput(bool keyboard[], struct player* player) {
    if (keyboard['W']) {
        float ty = player->pos.y + sin(player->lookangle)*player->speed;
        float tx = player->pos.x + cos(player->lookangle)*player->speed;
        player->pos.y = ty;
        player->pos.x = tx;
        if (player->pos.y >= 1) player->pos.y = 0;
    }
    if (keyboard['S']) {
        player->pos.y -= sin(player->lookangle)*player->speed;
        player->pos.x -= cos(player->lookangle)*player->speed;
        if (player->pos.y < 0) player->pos.y = 1;
    }
    if (keyboard['A']) {
        player->pos.y += sin(player->lookangle+PI/2)*player->speed;
        player->pos.x += cos(player->lookangle+PI/2)*player->speed;
        if (player->pos.x < 0) player->pos.x = 1;
    }
    if (keyboard['D']) {
        player->pos.y -= sin(player->lookangle+PI/2)*player->speed;
        player->pos.x -= cos(player->lookangle+PI/2)*player->speed;
        if (player->pos.x >= 1) player->pos.x = 0;
    }

    /* LOOK DIRECTION */
    if (keyboard['%']) {
        player->lookangle += player->turnspeed;
        if (player->lookangle >= PI - 0.01) player->lookangle = -PI; + player->turnspeed; 
        printf("Turning Left: %f\n", player->lookangle);
    }
    if (keyboard['\'']) {
        player->lookangle -= player->turnspeed;
        if (player->lookangle <= -PI + 0.01) player->lookangle = PI; - player->turnspeed; 
        printf("Turning Right: %f\n", player->lookangle);
    }


    /* SETTINGS */
    if (keyboard['N']) {
        player->fov--;
        printf("Decremented FOV: %i\n", player->fov);
    } 
    if (keyboard['M']) { 
        player->fov++;
        printf("Incremented FOV: %i\n", player->fov);
    }
    if (keyboard[188]) { // "." on Keyboard
        player->raycount--;
        printf("Decremented Rays: %i\n", player->raycount);
    }
    if (keyboard[190]) { // "," on Keyboard
        player->raycount++;
        printf("Incremented Rays: %i\n", player->raycount);
    }
}

void HandleMouseInput(const char input, struct player* player) {


    float speed = 0.01;
    return;
    switch (input) {
        /* LOOK DIRECTION */
        case '%': {
            player->lookangle += player->turnspeed;
            if (player->lookangle >= PI - 0.01) player->lookangle = -PI; + player->turnspeed; 
            printf("Turning Left: %f\n", player->lookangle);
        } break;
        case '\'': {
            player->lookangle -= player->turnspeed;
            if (player->lookangle <= -PI + 0.01) player->lookangle = PI; - player->turnspeed; 
            printf("Turning Right: %f\n", player->lookangle);
        } break;
    }
}

