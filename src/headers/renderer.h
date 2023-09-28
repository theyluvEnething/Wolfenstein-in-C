#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "frame.h"
#include "level.h"
#include "player.h"
#include "const.h"

/* TO-DO:

- OPTIMISE LINE RENDERER BY NOT DRAWING OVER ALREADY DRAWN PIXELS 
1

*/

void update_screen();
void background(int color, struct frame* frame);
void draw_pixel(struct vector2 pos, int color, struct frame* frame);
bool is_minimized(struct frame* frame);

/* FORMS */
void draw_rectangle(struct vector2 pos, struct vector2 size, int color, struct frame* frame);
void draw_circle(struct vector2 pos, int rad, int color, struct frame* frame);
void draw_center_circle(struct vector2 pos, int rad, int color, struct frame* frame);
void draw_pi_circle(struct vector2 pos, int rad, int color, struct frame* frame);
void draw_line(struct vector2 start_point, struct vector2 end_point, int width, int color, struct frame* frame);

/* WIREFRAME */
void draw_rectangle_wireframe(struct vector2 pos, struct vector2 size, int line_width, int color, struct frame* frame);
void draw_rectangle_wireframe_filled(struct vector2 pos, struct vector2 size, int line_width, int color, struct frame* frame);
void draw_rectangle_wireframe_point(struct vector2 start_point, struct vector2 end_point, int line_width, int color, struct frame* frame);



/* 3D */
void draw_cube(struct vector2 pos, struct vector2 size, int linewidth, int color, struct frame* frame);

void print_pointer(struct frame* frame);
void wash_machine(struct vector2 fix_point, struct vector2* spin_vector, int rad, float time);

void db_map_level(struct level* level, struct frame* debug);
void mn_map_level(struct level* level, struct frame* frame);

/* PlAYER */
void draw_player(struct player* player, struct level* level, struct frame* frame);
void raycast(struct player* player, struct level* level, struct frame* frame);

#endif