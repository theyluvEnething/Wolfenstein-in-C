#include <stdio.h>
#include <math.h>

#include "headers/input.h"
#include "headers/const.h"

void HandlePlayerInput(const char input, struct player* player) {

    // if (player->pos.x >= 1) player->pos.x = 0;
    // if (player->pos.x < 0) player->pos.x = 1;
    // if (player->pos.y >= 1) player->pos.y = 0;
    // if (player->pos.y < 0) player->pos.y = 1;

    switch (input) {

        /* MOVEMENT */
        case 'W': {
            player->pos.y +=0.01;
            if (player->pos.y >= 1) player->pos.y = 0;
        } break;
        case 'S': {
            player->pos.y -= 0.01;
            if (player->pos.y < 0) player->pos.y = 1;
        } break;
        case 'A': {
            player->pos.x -= 0.01;
            if (player->pos.x < 0) player->pos.x = 1;
        } break;
        case 'D': {
            player->pos.x += 0.01;
            if (player->pos.x >= 1) player->pos.x = 0;
        } break;

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


        /* SETTINGS */
        case 'N': {
            player->fov--;
            printf("Decremented FOV: %i\n", player->fov);
        } break;
        case 'M': { 
            player->fov++;
            printf("Incremented FOV: %i\n", player->fov);
        } break;
        case (char)188: { // "." on Keyboard
            player->raycount--;
            printf("Decremented Rays: %i\n", player->raycount);
        } break;
        case (char)190: { // "," on Keyboard
            player->raycount++;
            printf("Incremented Rays: %i\n", player->raycount);
        } break;
    }
}
