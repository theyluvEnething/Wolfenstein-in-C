#include <stdio.h>
#include <stdlib.h>
#include "headers/level.h"

/* LEVEL */
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
    level->objects = (struct object**)calloc((cols), sizeof(struct object*));
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
            level->objects[i][j].pos = vec2(j, i);
            level->objects[i][j].size = vec2(1, 1);
            char c = level->map[cols-i-1][j];
            if (c == '-') {
                level->objects[i][j].type = EMPTY;
            }
            else if (c == '#') {
                level->objects[i][j].type = WALL;
            }
            else if (c == '+') {
                level->objects[i][j].type = SQUARE;
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


/* OBJECTS */
bool check_collision(struct vector2 pos, struct object obj) {

    return true;

    bool nice =  pos.x >= obj.pos.x || pos.x <= obj.pos.x+obj.size.x || pos.y >= obj.pos.y || pos.y <= obj.pos.y+obj.size.y;
    printf("%s", nice);

    return pos.x >= obj.pos.x || pos.x <= obj.pos.x+obj.size.x || pos.y >= obj.pos.y || pos.y <= obj.pos.y+obj.size.y;
}

bool check_obstacle_at_position(struct vector2 estimatedPosition, struct level* level, struct frame* frame) {
    struct vector2 aPos = multiply_vector2(estimatedPosition, 1/(double)frame->width);
    int i = level->width*aPos.x;
    int j = level->width*aPos.y;
    printf("%f, %f, index: %i\n", aPos.x, aPos.y);
}



int get_object_size(struct level level) {
    return level.height*level.width;
}