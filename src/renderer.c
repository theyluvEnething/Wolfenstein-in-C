#include <stdbool.h>
#include <Windows.h>
#include <math.h>

#include "headers/func.h"
#include "headers/level.h"
#include "headers/vector2.h"
#include "headers/player.h"
#include "headers/renderer.h"

int get_direction(struct vector2 point1, struct vector2 point2);


/* GENERAL METHODS */
void update_screen(HWND* window_handle) {
    InvalidateRect(*window_handle, NULL, false);
    UpdateWindow(*window_handle);
}

void background(int color, struct frame* frame) {
    for (int i = 0; i < frame->width*frame->height; i++) {
        frame->pixels[i] = color;
    }
}

void draw_pixel(struct vector2 pos, int color, struct frame* frame) {
    frame->pixels[(int)(pos.x+pos.y*frame->width)%(frame->width*frame->height)] = color;
}

bool is_minimized(struct frame* frame) {
    return frame->width == 0 || frame->height == 0;
}



/* DRAWING METHODS */
void draw_rectangle(struct vector2 pos, struct vector2 size, int color, struct frame* frame) {

    int left = (size.x >= 0) ? pos.x : pos.x + size.x;
    int right = (size.x >= 0) ? pos.x + size.x : pos.x;
    int bottom = (size.y >= 0) ? pos.y : pos.y + size.y;
    int top = (size.y >= 0) ? pos.y + size.y : pos.y;

    #pragma region 
    // printf("Height: %i\n", height);
    // printf("Left: %i\n", left);
    // printf("Right: %i\n", right);
    // printf("Bottom: %i\n", bottom);
    // printf("Top: %i\n\n", top);
    #pragma endregion

    for (int y=bottom;y<=top;y++) {
        for (int x=left;x<=right;x++) {
            frame->pixels[(x+frame->width*y)%(frame->height*frame->width)] = color;
        }
    }
}

void draw_rectangle_wireframe(struct vector2 pos, struct vector2 size, int line_width, int color, struct frame* frame) {

    int left = (size.x >= 0) ? pos.x : pos.x + size.x;
    int right = (size.x >= 0) ? pos.x + size.x : pos.x;
    int bottom = (size.y >= 0) ? pos.y : pos.y + size.y;
    int top = (size.y >= 0) ? pos.y + size.y : pos.y;

    #pragma region 
    // printf("Height: %i\n", height);
    // printf("Left: %i\n", left);
    // printf("Right: %i\n", right);
    // printf("Bottom: %i\n", bottom);
    // printf("Top: %i\n\n", top);
    #pragma endregion

    draw_line(vector2(left, bottom),    vector2(right, bottom),     line_width, color, frame);
    draw_line(vector2(left, top),       vector2(right, top),        line_width, color, frame);
    draw_line(vector2(left, bottom),    vector2(left, top),         line_width, color, frame);
    draw_line(vector2(right, bottom),   vector2(right, top),        line_width, color, frame);
}

void draw_rectangle_wireframe_filled(struct vector2 pos, struct vector2 size, int line_width, int color, struct frame* frame) {

    int left = (size.x >= 0) ? pos.x : pos.x + size.x;
    int right = (size.x >= 0) ? pos.x + size.x : pos.x;
    int bottom = (size.y >= 0) ? pos.y : pos.y + size.y;
    int top = (size.y >= 0) ? pos.y + size.y : pos.y;

    #pragma region 
    // printf("Height: %i\n", height);
    // printf("Left: %i\n", left);
    // printf("Right: %i\n", right);
    // printf("Bottom: %i\n", bottom);
    // printf("Top: %i\n\n", top);
    #pragma endregion

    draw_line(vector2(left, bottom),    vector2(right, bottom),     line_width, color, frame);
    draw_line(vector2(left, top),       vector2(right, top),        line_width, color, frame);
    draw_line(vector2(left, bottom),    vector2(left, top),         line_width, color, frame);
    draw_line(vector2(right, bottom),   vector2(right, top),        line_width, color, frame);
    draw_line(vector2(left, bottom),    vector2(right, top),        line_width, color, frame);
    draw_line(vector2(left, top),       vector2(right, bottom),     line_width, color, frame);
}


void draw_circle(struct vector2 pos, int rad, int color, struct frame* frame) {
    for (int y=pos.y;y<pos.y+rad;y++) {
        for (int x=pos.x;x<pos.x+rad;x++) {
            if (int_distance(x, y, pos.x+rad/2, pos.y+rad/2) >= rad/2 || x+frame->width*y < 0) 
                continue;

            frame->pixels[(x+frame->width*y)%(frame->width*frame->height)] = color;
        }
    }
}

void draw_center_circle(struct vector2 pos, int rad, int color, struct frame* frame) {
    for (int y=pos.y-rad;y<pos.y+rad;y++) {
        for (int x=pos.x-rad;x<pos.x+rad;x++) {
            if (int_distance(x, y, pos.x, pos.y) >= rad/2 || x+frame->width*y < 0 || frame->width == 0 || frame->height == 0) 
                continue;

            frame->pixels[(x+frame->width*y)%(frame->width*frame->height)] = color;
        }
    }
}

void draw_pi_circle(struct vector2 pos, int rad, int color, struct frame* frame) {
    static const double PI = 3.1415926535;
    double i, angle, x1, y1;

    for (i = 0; i < 360; i += 0.1)
    {
        angle = i;
        x1 = rad * cos(angle * PI / 180);
        y1 = rad * sin(angle * PI / 180);

        frame->pixels[(int)((pos.x + x1)*((pos.y+y1)*frame->width))%(frame->width*frame->height)] = color;
    }
}

void draw_line(struct vector2 start_point, struct vector2 end_point, int width, int color, struct frame* frame) {


    
    /* Calculate Lines*/
    int left = min(start_point.x, end_point.x);
    int right = max(start_point.x, end_point.x);
    int bottom = min(start_point.y, end_point.y);
    int top = max(start_point.y, end_point.y);
    float function = (float)(top-bottom)/(float)(right-left);
    int sign = get_direction(start_point, end_point);
    int manipulateX = (left == right) ? 0 : 1;

    #pragma region 
    // printf("Left: %i\n", left);
    // printf("Right: %i\n", right);
    // printf("Bottom: %i\n", bottom);
    // printf("Top: %i\n", top);
    // printf("Sign: %i\n", sign);
    // printf("Function: %f\n\n", function);
    #pragma endregion



    /* Draw Lines */
    if (function <= 1) {
        for (int x=left; x<right; x++) {
            int y = (sign > 0) ? (int)(x-left)*function : (int)(top-bottom)-(x-left)*function;
            //printf("(%i | %i)\n", (int)x, y);
            for (int i = -width/2; i <= width/2; i++) {
                int wPx = (int)(x+frame->width*(y+i+bottom));
                int vPx = (int)(x+i+frame->width*(y+bottom));
                if (wPx < 0 || vPx < 0) { continue; }
               
                frame->pixels[(wPx)%(frame->width*frame->height)] = color;
                frame->pixels[(vPx)%(frame->width*frame->height)] = color;
            }  
        }
    }
    else {
        for (int y=bottom; y<top; y++) {
            int x = (sign > 0) ? (int)(left-right)-(y-top)*(1/function)*-1 : (int)(left-right)-(y-bottom)*(1/function);
            //printf("(%i | %i)\n", (int)x, y);
            for (int i = -width/2; i <= width/2; i++) {

                if ((int)(right+(right-left)+x)+frame->width*(y+i) < 0 || (int)(right+(right-left)+x)+i+frame->width*(y) < 0) continue;
                
                frame->pixels[(int)((right+(right-left)+x)+frame->width*(y+i))%(frame->width*frame->height)] = color;
                frame->pixels[(int)((right+(right-left)+x)+i+frame->width*(y))%(frame->width*frame->height)] = color;
            }
        } 
    }
    /* Draw circles on end-points */
    draw_center_circle(start_point, width, color, frame);
    draw_center_circle(end_point, width, color, frame);
}

void wash_machine(struct vector2 fix_point, struct vector2* spin_vector, int rad, float time) {
    spin_vector->x = fix_point.x + cosf(time) * rad;
    spin_vector->y = fix_point.y + sinf(time) * rad; //+ sinf(time) * rad;
}

void draw_cube(struct vector2 pos, struct vector2 size, int linewidth, int color, struct frame* frame) {

    draw_line(vector2(pos.x, pos.y),          vector2(pos.x+size.x, pos.y),          linewidth, color, frame);
    draw_line(vector2(pos.x, pos.y),          vector2(pos.x, pos.y+size.y),          linewidth, color, frame);
    draw_line(vector2(pos.x+size.x, pos.y),   vector2(pos.x+size.x, pos.y+size.y),   linewidth, color, frame);
    draw_line(vector2(pos.x, pos.y+size.y),   vector2(pos.x+size.x, pos.y+size.y),   linewidth, color, frame);

    draw_line(vector2(pos.x+size.x, pos.y),         vector2(pos.x+size.x+size.x/2, pos.y+size.y/2),          linewidth, color, frame);
    draw_line(vector2(pos.x+size.x, pos.y+size.y),  vector2(pos.x+size.x+size.x/2, pos.y+size.y+size.y/2),   linewidth, color, frame);
    draw_line(vector2(pos.x, pos.y+size.y),         vector2(pos.x+size.x/2, pos.y+size.y+size.y/2),          linewidth, color, frame);

    draw_line(vector2(pos.x+size.x/2, pos.y+size.y+size.y/2),   vector2(pos.x+size.x+size.x/2, pos.y+size.y+size.y/2),   linewidth, color, frame);
    draw_line(vector2(pos.x+size.x+size.x/2, pos.y+size.y/2),   vector2(pos.x+size.x+size.x/2, pos.y+size.y+size.y/2),   linewidth, color, frame);
}

void print_pointer(struct frame *frame) {
    printf("========================\n");
    printf("Pointer: 0x%p\n", frame);
    printf("Window Width: %i\n", frame->width);
    printf("Window Height: %i\n", frame->height);
    printf("========================\n");
}

void db_map_level(struct level* level, struct frame* debug) {

    // Calculate size of squares
    float sqr_size = debug->width / (float)level->width;
    //printf("Square Size: %f\n", sqr_size);

    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) { 
            int x = level->objects[i][j].pos.x * sqr_size;
            int y = level->objects[i][j].pos.y * sqr_size;

            if (level->objects[i][j].type == EMPTY) continue;

            draw_rectangle_wireframe_filled(vector2(x, y), vector2(sqr_size, sqr_size), 3, 0xFFFFFF, debug);
        }
    }
}

void mn_map_level(struct level* level, struct frame* frame) {

}


/* PLAYER */
void draw_player(struct player* player, struct frame* frame) {
    draw_center_circle(player->pos, 50, 0xFF0000, frame);


    struct vector2 range = {0, 0};
    double turn_speed = 100;

    range.x = frame->width - (cosf(player->angle)*player->pos.x);
    range.y = 0;

    struct vector2 normalized_look_vector = normalize_vector(vector2(cosf(player->angle)*turn_speed, sinf(player->angle)*turn_speed));

    draw_line(player->pos, add_vector2(player->pos, multiply_vector2(normalized_look_vector, magnitude(range))), 10, 0xFF0000, frame);
    // printf("(%f, %f)\n", look_vector.x, look_vector.y);
    // printf("(%f, %f)\n", normalize_vector(look_vector).x, normalize_vector(look_vector).y);
}










/* OTHER METHODS */
int get_direction(struct vector2 point1, struct vector2 point2) {
    if (point1.x == point2.x || point1.y == point2.y) 
        return 1;

    if (point2.y > point1.y) {
        return (point2.x > point1.x) ? 1 : -1;
    }
    else {
        return (point2.x > point1.x) ? -1 : 1;
    }
}