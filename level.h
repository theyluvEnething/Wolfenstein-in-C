#ifndef LEVEL_H
#define LEVEL_H

struct level {
    int width;
    int height;
    char* map;
};

void init_Map(char** map, struct level* level);
void free_Map(struct level* level);

#endif