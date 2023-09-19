#include <stdio.h>
#include "level.h"


void init_Map(char** map, struct level* level) {


    int rows = {0}, cols = {0};

    while (map[rows] != NULL) {
        rows++;
    }
    while (map[0][cols] != '\0') {
        cols++;
    }
    printf("Rows: %i\n", rows);
    printf("Cols: %i\n", cols);

    for (int i = 0; i < rows; i++) {
        printf("[");
        for (int j = 0; j < cols; j++) {
            printf("%c", map[i][j]);
        }
        printf("]\n");
    }
}

// void free_Map(struct Level* level) {
//     for (int i = 0; i < level->width; i++) {
//         free(level->map[i]);
//     }
//     free(level->map);
// }

// void print_map(struct level* map) {
//     int i = 0;
//     while(*map->arr[i] != null) {
//         printf("%c\n", *map->arr[i]);
//         i++;
//     }
// }