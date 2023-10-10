#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include "frame.h"
#include "vector2.h"

struct level {
    int width;
    int height;
    char** map;
    struct object** objects;
};

struct object {
    struct vector2 pos;
    struct vector2 size;
    float height_offset;
    enum type {EMPTY = '-', WALL = '#', SQUARE = '+'} type;
};

void init_map(char** map, struct level* level);
void free_map(struct level* level);
void print_map(struct level* level);
void print_map_values(struct level* level);

int get_object_size(struct level level); 
bool check_collision(struct vector2 pos, struct object obj);
bool check_obstacle_at_position(struct vector2 estimatedPosition, struct level* level, struct frame* frame);

#endif