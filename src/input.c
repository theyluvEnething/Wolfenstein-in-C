#include <stdio.h>
#include <math.h>

#include "headers/input.h"

void HandlePlayerInput(const char input, struct player* player) {
    switch (input) {
        /* MOVEMENT */
        case 'W': {
            player->pos.y += 5;
        } break;
        case 'S': {
            player->pos.y -= 5;
        } break;
        case 'A': {
            player->pos.x -= 5;
        } break;
        case 'D': {
            player->pos.x += 5;
        } break;

        /* DIRECTION */
        case '%': {
            player->angle += 0.03;
            // printf("Look X: %f\n", (float)cosf(player->angle) * 100);
            // printf("Look Y: %f\n", (float)sinf(player->angle) * 100);
            printf("Turning Left: %f\n", player->angle);
        } break;
        case '\'': {
            player->angle -= 0.03;
            // printf("Look X: %f\n", (float)cosf(player->angle) * 100);
            // printf("Look Y: %f\n", (float)sinf(player->angle) * 100);
            printf("Turning Right: %f\n", player->angle);
        } break;
    }
}
