#include <stdio.h>
#include <stdlib.h>
#include "headers/level.h"
#include "headers/vector2.h"

void init_map(char** map, struct level* level) {

    level->map = map;

    int cols = {0}, rows = {0};

    while (level->map[cols] != NULL) cols++;
    while (level->map[0][rows] != '\0') rows++;

    level->height = cols;
    level->width = rows;

    printf("Rows: %i\n", cols);
    printf("Cols: %i\n", rows);

    printf("\nInitializing Obstacles!\n");
    /* INITIALIZE OBSTACLE ARRAY */
    level->objects = (struct object**)calloc((cols-1), sizeof(struct object*));
    for (int i = 0; i <= cols; i++) {
        level->objects[i] = (struct object*)calloc((rows), sizeof(struct object));
        if (level->objects[i] == NULL) {
            printf("Memory allocation for obstacles array failed.\n");
            exit(1); // Handle the error as needed
        }
    }

    /* Initialize obstacle array with map */
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            struct vector2 vec = {j, i};
            level->objects[i][j].pos = vec;
            char c = level->map[i][j];
            if (c != '#') {
                level->objects[i][j].type = EMPTY;
            }
        }
    }
    print_map_values(level);
    printf("\n");
}

void free_map(struct level* level) {
    for (int i = 0; i < level->height; i++) {
        free(level->objects[i]);
    }
    free(level->objects);
    level->objects = NULL;
}
void print_map_values(struct level* level) {
    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) {
            printf("[%i,%i] ", level->objects[i][j].pos.x, level->objects[i][j].pos.y);
        }
        printf("\n");
    }
}
void print_map(struct level* level) {
    for (int i = 0; i < level->height; i++) {
        printf("[");
        for (int j = 0; j < level->width; j++) {
            printf("%c", level->map[i][j]);
        }
        printf("]\n");
    }
}

