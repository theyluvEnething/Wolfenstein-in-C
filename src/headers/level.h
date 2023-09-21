#ifndef LEVEL_H
#define LEVEL_H

#include "vector2.h"

struct level {
    int width;
    int height;
    char** map;
    struct object** objects;
};

struct object {
    struct vector2 pos;
    enum type {EMPTY = '-', WALL = '#'} type;
};

void init_map(char** map, struct level* level);
void free_map(struct level* level);
void print_map(struct level* level);
void print_map_values(struct level* level);

#endif