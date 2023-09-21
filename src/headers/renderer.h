#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "frame.h"
#include "level.h"

/* TO-DO:

- OPTIMISE LINE RENDERER BY NOT DRAWING OVER ALREADY DRAWN PIXELS 


*/

void update_screen();
void background(int color, struct frame* frame);
void draw_pixel(int posX, int posY, int color, struct frame* frame);

/* FORMS */
void draw_rectangle(int posX, int posY, int width, int height, int color, struct frame* frame);
void draw_circle(int posX, int posY, int rad, int color, struct frame* frame);
void draw_center_circle(int posX, int posY, int rad, int color, struct frame* frame);
void draw_pi_circle(int posX, int posY, int rad, int color, struct frame* frame);
void draw_line(int x1, int y1, int x2, int y2, int width, int color, struct frame* frame);

/* WIREFRAME */
void draw_rectangle_wireframe(int posX, int posY, int width, int height, int line_width, int color, struct frame* frame);
void draw_rectangle_wireframe_filled(int posX, int posY, int width, int height, int line_width, int color, struct frame* frame);


/* 3D */
void draw_cube(int posX, int posY, int width, int height, int linewidth, int color, struct frame* frame);

void print_pointer(struct frame* frame);
void IntSpinner(int fixPoint_x, int fixPoint_y, int *spinX, int *spinY, int rad, float time);

void db_map_level(struct level* level, struct frame* debug);

#endif