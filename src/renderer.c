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

bool check_overlap(struct vector2 pos, struct frame* frame) {
    return (pos.x < 0 || pos.x >= frame->width || pos.y < 0 || pos.y >= frame->height);
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
            frame->pixels[clampI(x+frame->width*y, 0, frame->width*frame->height-1)] = color;
        }
    }
}

void draw_rectangle_from_points(struct vector2 point1, struct vector2 point2, int color, struct frame* frame) {
    
    int left = min(point1.x, point2.x);
    int right = max(point1.x, point2.x);
    int bottom = min(point1.y, point2.y);
    int top = max(point1.y, point2.y);

    left = left < 0 ? 0 : left;
    right = right > frame->width-1 ? frame->width-1 : right;
    bottom = bottom < 0 ? 0 : bottom;
    top = top > frame->height-1 ? frame->height-1 : top;

    #pragma region 
    // printf("Height: %i\n", height);
    // printf("Left: %i\n", left);
    // printf("Right: %i\n", right);
    // printf("Bottom: %i\n", bottom);
    // printf("Top: %i\n\n", top);
    #pragma endregion

    for (int y=bottom;y<=top;y++) {
        for (int x=left;x<=right;x++) {
            frame->pixels[x+frame->width*y] = color;
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

void draw_rectangle_wireframe_point(struct vector2 start_point, struct vector2 end_point, int line_width, int color, struct frame* frame) {

    int left = min(start_point.x, end_point.x);
    int right = max(start_point.x, end_point.x);
    int bottom = min(start_point.y, end_point.y);
    int top = max(start_point.y, end_point.y);

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
            if (distanceI(x, y, pos.x+rad/2, pos.y+rad/2) >= rad/2 || check_overlap(vec2(x, y), frame)) 
                continue;

            frame->pixels[clampI(x+frame->width*y, 0, frame->width*frame->height-1)] = color;
        }
    }
}

void draw_center_circle(struct vector2 pos, int rad, int color, struct frame* frame) {
    for (int y=pos.y-rad;y<pos.y+rad;y++) {
        for (int x=pos.x-rad;x<pos.x+rad;x++) {
            if (distanceI(x, y, pos.x, pos.y) >= rad/2 || check_overlap(vec2(x, y), frame)) 
                continue;

            frame->pixels[clampI(x+frame->width*y, 0, frame->width*frame->height-1)] = color;
        }
    }
}

void draw_pi_circle(struct vector2 pos, int rad, int color, struct frame* frame) {
    double i, angle, x1, y1;

    for (i = 0; i < 360; i += 0.1)
    {
        angle = i;
        x1 = rad * cos(angle * PI / 180);
        y1 = rad * sin(angle * PI / 180);

        //frame->pixels[clampI((pos.x + x1)*((pos.y+y1)*frame->width)0, frame->width*frame->height-1)] = color;
    }
}

/*  ENETHS LINE ALGORITHM


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

    if (function <= 1) {
        for (int x=left; x<right; x++) {

            int y = (sign > 0) ? (int)(x-left)*function : (int)(top-bottom)-(x-left)*function;
            if (y >= frame->height || x >= frame->width) { continue; }

            for (int i = -width/2; i <= width/2; i++) {
                uint32_t wPx = (int)(x+frame->width*(y+i+bottom));
                uint32_t vPx = (int)(x+i+frame->width*(y+bottom));
                frame->pixels[(wPx)%(frame->width*frame->height)] = color;
                frame->pixels[(vPx)%(frame->width*frame->height)] = color;
            }  
        }
    }
    else {
        for (int y=bottom; y<top; y++) {

            int x = (sign > 0) ? (int)(left-right)-(y-top)*(1/function)*-1 : (int)(left-right)-(y-bottom)*(1/function);
            if (y >= frame->height || x >= frame->width) { continue; }

            for (int i = -width/2; i <= width/2; i++) {
                uint16_t wPx = (uint16_t)(right+(right-left)+x)+frame->width*(y+i);
                uint16_t vPx = (uint16_t)(right+(right-left)+x)+i+frame->width*(y);

                frame->pixels[(wPx)%(frame->width*frame->height)] = color;
                frame->pixels[(vPx)%(frame->width*frame->height)] = color;
            }
        } 
    }
*/

void draw_line(struct vector2 start_point, struct vector2 end_point, int width, int color, struct frame* frame) {
    
    int dx = abs(end_point.x - start_point.x);
    int dy = abs(end_point.y - start_point.y);
    bool slope = dy > dx;

    if (slope) {
        int temp = start_point.x;
        start_point.x = start_point.y;
        start_point.y = temp;

        temp = end_point.x;
        end_point.x = end_point.y;
        end_point.y = temp;
    }
    if (start_point.x > end_point.x) {
        int temp = start_point.x;
        start_point.x = end_point.x;
        end_point.x = temp;

        temp = start_point.y;
        start_point.y = end_point.y;
        end_point.y = temp;
    }

    dx = abs((int)(end_point.x - start_point.x));
    dy = abs((int)(end_point.y - start_point.y));
    float error = dx / 2;
    int y = start_point.y;
    int ystep =  start_point.y < end_point.y ? 1 : -1;
    for (int x = start_point.x; x < end_point.x; x++) {

        //x = x > frame->width-1 ? frame->width-1 : x;
        //y = y > frame->height-1 ? frame->height-1 : y;
        //printf("%i\n", y);
        struct vector2 pos = slope ? vec2(y, x) : vec2(x, y);

        for (int i = -width/2; i < width/2+1; i++) {
            if (pos.x + i < 0 || pos.x + i > frame->width-1 || pos.y + i < 0 || pos.y + i > frame->height-1) continue;
            frame->pixels[(uint32_t)(pos.x+i+frame->width*pos.y)%(frame->height*frame->width)] = color;
            frame->pixels[(uint32_t)(pos.x+frame->width*(pos.y+i))] = color;
        }

        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
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

/* OTHER METHODS */
void print_frame(struct frame *frame) {
    printf("========================\n");
    printf("Pointer: 0x%p\n", frame);
    printf("Window Width: %i\n", frame->width);
    printf("Window Height: %i\n", frame->height);
    printf("========================\n");
}

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


void draw_level2D(struct player* player, struct level* level, struct frame* debug) {

    // Calculate size of squares
    float sqr_size = debug->width / (float)level->width;
    //printf("Square Size: %f\n", sqr_size);

    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) { 
            int x = level->objects[i][j].pos.x * sqr_size;
            int y = level->objects[i][j].pos.y * sqr_size;

            //printf("(%i, %i)\n", x, y);

            if (level->objects[i][j].type == EMPTY) continue;
            else if (level->objects[i][j].type == WALL) draw_rectangle_wireframe_filled(vector2(x, y), vector2(sqr_size, sqr_size), 3, 0xFFFFFF, debug);
            else if (level->objects[i][j].type == SQUARE) draw_rectangle_wireframe(vector2(x, y), vector2(sqr_size, sqr_size), 3, 0xFFFFFF, debug);
        }
    }

    draw_player2D(player, level, debug);
}

void draw_level3D(struct player* player, struct level* level, struct frame* frame) {
    //reversecast3D(player, level, frame);
    raycast3D(player, level, frame);
}


/* PLAYER */
void draw_player2D(struct player* player, struct level* level, struct frame* frame) {

    //printf("%f, %f\n", pPos.x, pPos.y);
    draw_center_circle(vector2(player->pos.x*frame->width, player->pos.y*frame->height), 10, 0xFF0000, frame);
    raycast2D(player, level, frame);
}

void raycast2D(struct player* player, struct level* level, struct frame* frame) {
    struct vector2 pPos = vector2(player->pos.x*frame->width, player->pos.y*frame->height);
    int max_range = distanceI(0, 0, frame->width, frame->height);

    float sqr_size = frame->width / (float)level->width;
    struct vector2 pSqr;
    pSqr.x = clampI(pPos.x/sqr_size, 0, level->width-1);
    pSqr.y = clampI(pPos.y/sqr_size, 0, level->height-1);
    
    struct vector2 sqrDiff;

    sqrDiff.x = sqr_size - (int)pPos.x % (int)sqr_size;
    sqrDiff.y = sqr_size - (int)pPos.y % (int)sqr_size;

    float d_x = 0;
    float d_y = 0;

    struct object checkObj = level->objects[(int)(5-pSqr.y)][(int)pSqr.x];
    draw_rectangle_wireframe_point(vec2(pPos.x-(sqr_size-sqrDiff.x), pPos.y-(sqr_size-sqrDiff.y)), vec2(pPos.x+sqrDiff.x, pPos.y+sqrDiff.y), 5, 0x00FFFF, frame);

    if (level->objects[(int)pSqr.y][(int)pSqr.x].type != '-') return;

    int total_calc = 0;
    for (int i = -player->raycount/2; i <= player->raycount/2; i++) {
        
        //struct vector2 range = {0, 0};
        int raycount = player->raycount % 2 == 0 ? player->raycount : player->raycount-1;
        float angle_diff = i*(PI*(player->fov/(float)180)/(raycount)); 
        double ray_angle = player->lookangle + angle_diff;
        struct vector2 normalized_look_vector = normalize_vector(vector2(cosf(ray_angle), sinf(ray_angle)));


        int range = 0;
        int limit = 0;
        const int hard_limit = 200;
        while (range <= max_range && limit < hard_limit) {

            // FIX CRASH

            struct vector2 grid;
            grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
            grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);


            /* INTERPOLATION METHOD */
            if (level->objects[(int)grid.y][(int)grid.x].type != '-') {                 
                
                range -= sqr_size/4;
                grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);

                while (level->objects[(int)grid.y][(int)grid.x].type != '-' && limit < hard_limit) {
                    range -= sqr_size/8;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                while (level->objects[(int)grid.y][(int)grid.x].type == '-' && limit < hard_limit) {
                    range += sqr_size/16;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                while (level->objects[(int)grid.y][(int)grid.x].type != '-' && limit < hard_limit) {
                    range -= 1;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                break;
            }
            range += sqr_size/4;
        
            total_calc++;
            limit++;
        }

    
        draw_line(pPos, add_vector2(pPos, multiply_vector2(normalized_look_vector, range)), 1, 0xFF00FF, frame);

        #pragma region 
        // printf("Look Angle: %.2f | ", ray_angle);
        // printf("Player Pos: (%1.f, %1.f) | ", pPos.x, pPos.y);
        // printf("Look Point: (%1.f, %1.f) | ", add_vector2(player->pos, multiply_vector2(normalized_look_vector, magnitude(range))).x, add_vector2(player->pos, multiply_vector2(normalized_look_vector, magnitude(range))).y);
        // printf(" cos: %.2f | sin: %.2f", cosf(ray_angle), sinf(ray_angle));
        // printf("\n");
        #pragma endregion
    }

    #pragma region
    // printf("Player Square: (%.2f | %.2f) | ", pSqr.x, 5-pSqr.y);
    // printf("Square Diff: (%.2f | %.2f) | ", sqrDiff.x, sqrDiff.y);
    // printf("Sqr Start: (%.2f | %.2f) | ", pPos.x-(sqr_size-d_x), pPos.y-(sqr_size-d_y));
    // printf("Square End: (%.2f | %.2f) | ", pPos.x+d_x, pPos.y+d_y);
    // printf("Calc: %i", total_calc);
    // printf("\n");
    #pragma endregion
}



void raycast3D(struct player* player, struct level* level, struct frame* frame) {
    struct vector2 pPos = vector2(player->pos.x*frame->width, player->pos.y*frame->height);
    const int max_range = distanceI(0, 0, frame->width, frame->height);

    const float sqr_size = frame->width / (float)level->width;
    struct vector2 pSqr;
    pSqr.x = clampI(pPos.x/sqr_size, 0, level->width-1);
    pSqr.y = clampI(pPos.y/sqr_size, 0, level->height-1);
    
    struct vector2 sqrDiff;

    sqrDiff.x = sqr_size - (int)pPos.x % (int)sqr_size;
    sqrDiff.y = sqr_size - (int)pPos.y % (int)sqr_size;

    float d_x = 0;
    float d_y = 0;

    if (level->objects[(int)pSqr.y][(int)pSqr.x].type != '-') return;


    int raycount = player->raycount % 2 == 0 ? player->raycount : player->raycount-1;
    float pixel_diff = frame->width/(float)raycount;
    int max_height = frame->height/18; // frame->height/8;
    
    int total_calc = 0;
    for (int i = -raycount/2; i <= raycount/2; i++) {
        /* VARIABLES */
        float angle_diff = i*(PI*(player->fov/(float)180)/(raycount)); 
        double ray_angle = player->lookangle + angle_diff;
        struct vector2 normalized_look_vector = normalize_vector(vector2(cosf(ray_angle), sinf(ray_angle)));


        int range = 0;
        int limit = 0;
        const int hard_limit = 200;
        
        struct vector2 grid;
        while (range <= max_range && limit < hard_limit) {

            // FIX CRASH

            grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
            grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);


            /* INTERPOLATION METHOD */
            if (level->objects[(int)grid.y][(int)grid.x].type != '-') {                 
                
                range -= sqr_size/4;
                grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);

                while (level->objects[(int)grid.y][(int)grid.x].type != '-' && limit < hard_limit) {
                    range -= sqr_size/8;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                while (level->objects[(int)grid.y][(int)grid.x].type == '-' && limit < hard_limit) {
                    range += sqr_size/16;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                while (level->objects[(int)grid.y][(int)grid.x].type != '-' && limit < hard_limit) {
                    range -= 1;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                range += 1;
                grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    
                break;
            }
            range += sqr_size/4;

            // MAKE BIG STEPS (AS BIG AS SQUARE-1) AND THEN ADD CORRECTION

            /* CLEAN METHOD */
            // if (level->objects[(int)grid.y][(int)grid.x].type == '#') {
            //     break;
            // } 
            // range+=1;
            limit++;
            total_calc++;
        }

        float line_x = frame->width/2-(pixel_diff * i);
        float percent = 1-(100/(float)max_range)*range/100;

        int r = 255*(percent*percent)+0.1;
        int g = 255*(percent*percent)+0.1;
        int b = 255*(percent*percent)+0.1;

        unsigned int hex = 0x000000 | (r << 16) | (g << 8) | (b << 0);

        int color = hex;
        //printf("%i\n", limit);
        // printf("%i\n", 0xFFFFFF);
        float height_multiplier = (max_range/(float)(range*max_height));
        float offset = level->objects[(int)grid.y][(int)grid.x].height_offset;
        draw_rectangle_from_points(vec2(line_x, offset*height_multiplier+frame->height/2-frame->height*height_multiplier), vec2(line_x + pixel_diff, offset*height_multiplier+frame->height/2+frame->height*height_multiplier), color, frame);

        #pragma region 
        // printf("Look Angle: %.2f | ", ray_angle);
        // printf("Player Pos: (%1.f, %1.f) | ", pPos.x, pPos.y);
        // printf("Look Point: (%1.f, %1.f) | ", add_vector2(player->pos, multiply_vector2(normalized_look_vector, magnitude(range))).x, add_vector2(player->pos, multiply_vector2(normalized_look_vector, magnitude(range))).y);
        // printf(" cos: %.2f | sin: %.2f", cosf(ray_angle), sinf(ray_angle));
        // printf("\n");
        #pragma endregion
    }

    #pragma region
    // printf("Player Square: (%.2f | %.2f) | ", pSqr.x, 5-pSqr.y);
    // printf("Square Diff: (%.2f | %.2f) | ", sqrDiff.x, sqrDiff.y);
    // printf("Sqr Start: (%.2f | %.2f) | ", pPos.x-(sqr_size-d_x), pPos.y-(sqr_size-d_y));
    // printf("Square End: (%.2f | %.2f) | ", pPos.x+d_x, pPos.y+d_y);
    // printf("Calc: %i", total_calc);
    // printf("\n");
    #pragma endregion
}


int Partition(struct vector2* arr, int start, int end) {
    int pivot = arr[end].y;
    int i = start-1;

    for (int j = start; j <= end; j++) {
        if (arr[j].y < pivot) {
            i++;
            struct vector2 temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    i++;
    
    struct vector2 temp = arr[i];
    arr[i] = arr[end];
    arr[end] = temp;

    return i;
}

void quicksort(struct vector2* arr, int start, int end) {

    if (start <= end) {
        return;
    }

    int pivot = Partition(arr, start, end);
    quicksort(arr, start, pivot-1);
    quicksort(arr, pivot+1, end);
}






#define clamp(a, minVal, maxVal) min(max(a, minVal), maxVal)

void reversecast3D(struct player* player, struct level* level, struct frame* frame) {

    int buffersize = 4096;
    struct vector2 *depthBuffer = calloc(sizeof(struct vector2), buffersize+1); 
    struct vector2 *hitObjects = calloc(sizeof(struct vector2), buffersize+1); 


    struct vector2 pPos = vector2(player->pos.x*frame->width, player->pos.y*frame->height);
    const int max_range = distanceI(0, 0, frame->width, frame->height);

    const float sqr_size = frame->width / (float)level->width;
    struct vector2 pSqr;
    pSqr.x = clampI(pPos.x/sqr_size, 0, level->width-1);
    pSqr.y = clampI(pPos.y/sqr_size, 0, level->height-1);
    
    struct vector2 sqrDiff;

    sqrDiff.x = sqr_size - (int)pPos.x % (int)sqr_size;
    sqrDiff.y = sqr_size - (int)pPos.y % (int)sqr_size;

    float d_x = 0;
    float d_y = 0;

    if (level->objects[(int)pSqr.y][(int)pSqr.x].type != '-') return;


    int raycount = player->raycount % 2 == 0 ? player->raycount : player->raycount-1;
    float pixel_diff = frame->width/(float)raycount;
    int max_height = frame->height/18; // frame->height/8;
    
    int total_calc = 0;
    int n = 0;
    for (int i = -raycount/2; i <= raycount/2; i++) {
        /* VARIABLES */
        float angle_diff = i*(PI*(player->fov/(float)180)/(raycount)); 
        double ray_angle = player->lookangle + angle_diff;
        struct vector2 normalized_look_vector = normalize_vector(vector2(cosf(ray_angle), sinf(ray_angle)));


        int range = 0;
        int limit = 0;
        const int hard_limit = 200;
        
        struct vector2 grid;
        while (range <= max_range && limit < hard_limit && n < buffersize) {

            // FIX CRASH

            grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
            grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);


            /* INTERPOLATION METHOD */
            if (level->objects[(int)grid.y][(int)grid.x].type != '-') {                 
                
                range -= sqr_size/4;
                grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);

                while (level->objects[(int)grid.y][(int)grid.x].type != '-' && limit < hard_limit) {
                    range -= sqr_size/8;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                while (level->objects[(int)grid.y][(int)grid.x].type == '-' && limit < hard_limit) {
                    range += sqr_size/16;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                while (level->objects[(int)grid.y][(int)grid.x].type != '-' && limit < hard_limit) {
                    range -= 1;
                    grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                    grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    total_calc++;
                    limit++;
                }
                range += 1;
                grid.x = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).x/sqr_size))%(level->width+1);
                grid.y = ((uint8_t)(add_vector2(pPos, multiply_vector2(normalized_look_vector, range)).y/sqr_size))%(level->height+1);
                    
                depthBuffer[n] = (struct vector2) {
                    frame->width/2-(pixel_diff * i),
                    range,
                };
                
                hitObjects[n] = grid;

                n++;
                range += sqr_size;
            }
            range += sqr_size/4;

            // MAKE BIG STEPS (AS BIG AS SQUARE-1) AND THEN ADD CORRECTION

            /* CLEAN METHOD */
            // if (level->objects[(int)grid.y][(int)grid.x].type == '#') {
            //     break;
            // } 
            // range+=1;
            limit++;
            total_calc++;
        }


        #pragma region 
        // printf("Look Angle: %.2f | ", ray_angle);
        // printf("Player Pos: (%1.f, %1.f) | ", pPos.x, pPos.y);

        // printf("Look Point: (%1.f, %1.f) | ", add_vector2(player->pos, multiply_vector2(normalized_look_vector, magnitude(range))).x, add_vector2(player->pos, multiply_vector2(normalized_look_vector, magnitude(range))).y);
        // printf(" cos: %.2f | sin: %.2f", cosf(ray_angle), sinf(ray_angle));
        // printf("\n");
        #pragma endregion
    }

        //float line_x = frame->width/2-(pixel_diff * i);

        // bool sorted = false;
        // int iter = 0;
        // while (!sorted) { 
        //     sorted = true;
        //     for (int i = buffersize-1; i > -1; i--) {
        //         if (depthBuffer[i].y < depthBuffer[i+1].y && depthBuffer[i].y != 0 && depthBuffer[i+1].y != 0) {
        //             printf("[%i | (%.2f, %.2f)]\n", iter, depthBuffer[i].y, depthBuffer[i+1].y);
        //             sorted = false;
        //             const struct vector2 temp = depthBuffer[i];
        //             depthBuffer[i] = depthBuffer[i+1];
        //             depthBuffer[i] = temp;
        //         }
        //     }
        //     iter++;
        // }

        quicksort(depthBuffer, 0, buffersize-1);


        //printf("%i\n", limit);
        // printf("%i\n", 0xFFFFFF);
        // float height_multiplier = (max_range/(float)(range*max_height));
        // float offset = level->objects[(int)grid.y][(int)grid.x].height_offset;
        // draw_rectangle_from_points(vec2(line_x, offset*height_multiplier+frame->height/2-frame->height*height_multiplier), vec2(line_x + pixel_diff, offset*height_multiplier+frame->height/2+frame->height*height_multiplier), color, frame);

        // for (int i = buffersize-1; i > -1; i--) {
            
        //     float percent = 1-(100/(float)max_range)*depthBuffer[i].y/100;
        //     int r = 255*(percent*percent)+0.1;
        //     int g = 255*(percent*percent)+0.1;
        //     int b = 255*(percent*percent)+0.1;
        //     unsigned int hex = 0x000000 | (r << 16) | (g << 8) | (b << 0);
        //     int color = hex;

        //     float height_multiplier = (max_range/(float)(depthBuffer[i].y*max_height));
        //     float offset = level->objects[(int)hitObjects[i].y][(int)hitObjects[i].x].height_offset;

        //     draw_rectangle_from_points(vec2(depthBuffer[i].x, offset*height_multiplier+frame->height/2-frame->height*height_multiplier), vec2(depthBuffer[i].x + pixel_diff, offset*height_multiplier+frame->height/2+frame->height*height_multiplier), color, frame);
        // }

        for (int i = 0; i < buffersize; i++) {
            
            float percent = 1-(100/(float)max_range)*depthBuffer[i].y/100;
            int r = 255*(percent*percent)+0.1;
            int g = 255*(percent*percent)+0.1;
            int b = 255*(percent*percent)+0.1;
            unsigned int hex = 0x000000 | (r << 16) | (g << 8) | (b << 0);
            int color = hex;

            float height_multiplier = (max_range/(float)(depthBuffer[i].y*max_height));
            float offset = level->objects[(int)hitObjects[i].y][(int)hitObjects[i].x].height_offset;

            draw_rectangle_from_points(vec2(depthBuffer[i].x, offset*height_multiplier+frame->height/2-frame->height*height_multiplier), vec2(depthBuffer[i].x + pixel_diff, offset*height_multiplier+frame->height/2+frame->height*height_multiplier), color, frame);
        }

    #pragma region
    // printf("Player Square: (%.2f | %.2f) | ", pSqr.x, 5-pSqr.y);
    // printf("Square Diff: (%.2f | %.2f) | ", sqrDiff.x, sqrDiff.y);
    // printf("Sqr Start: (%.2f | %.2f) | ", pPos.x-(sqr_size-d_x), pPos.y-(sqr_size-d_y));
    // printf("Square End: (%.2f | %.2f) | ", pPos.x+d_x, pPos.y+d_y);
    // printf("Calc: %i", total_calc);
    // printf("\n");
    #pragma endregion
}
