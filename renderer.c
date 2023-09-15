#include <stdbool.h>
#include <Windows.h>
#include <math.h>
#include "renderer.h"
#include "func.h"

int getDirection(float x1, float y1, float x2, float y2);

void update_screen(HWND* window_handle) {
    InvalidateRect(*window_handle, NULL, false);
    UpdateWindow(*window_handle);
}

void background(int color, struct frame* frame) {
    for (int i = 0; i < frame->width*frame->height; i++) {
        frame->pixels[i] = color;
    }
}

void draw_rectangle(int posX, int posY, int width, int height, int color, struct frame* frame) {

    int left = (width >= 0) ? posX : posX + width;
    int right = (width >= 0) ? posX + width : posX;
    int bottom = (height >= 0) ? posY : posY + height;
    int top = (height >= 0) ? posY + height : posY;

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

void draw_circle(int posX, int posY, int rad, int color, struct frame* frame) {
    for (int y=posY;y<posY+rad;y++) {
        for (int x=posX;x<posX+rad;x++) {

            if (int_distance(x, y, posX+rad/2, posY+rad/2) >= rad/2) 
                continue;

            frame->pixels[(x+frame->width*y)%(frame->width*frame->height)] = color;
        }
    }
}

void draw_center_circle(int posX, int posY, int rad, int color, struct frame* frame) {
    for (int y=posY-rad;y<posY+rad;y++) {
        for (int x=posX-rad;x<posX+rad;x++) {
            if (int_distance(x, y, posX, posY) >= rad/2) 
                continue;

            frame->pixels[(x+frame->width*y)%(frame->width*frame->height)] = color;
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2, int width, int color, struct frame* frame) {

    /* Calculate Lines*/
    int left = min(x1, x2);
    int right = max(x1, x2);
    int bottom = min(y1, y2);
    int top = max(y1, y2);
    float function = (float)(top-bottom)/(right-left);
    int sign = getDirection(x1, y1, x2, y2);
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
        for (float x=left; x<right; x++) {
            int y = (sign > 0) ? (int)(x-left)*function : (int)(top-bottom)-(x-left)*function;
            //printf("(%i | %i)\n", (int)x, y);
            for (int i = -width/2; i <= width/2; i++) {
                frame->pixels[(int)(x+frame->width*(y+i+bottom))%(frame->width*frame->height)] = color;
            }
            for (int j = -width/2; j < width/2; j++) {
                frame->pixels[(int)(x+j+frame->width*(y+bottom))%(frame->width*frame->height)] = color;
            }   
        }
    } else {
        for (float y=bottom; y<top; y++) {
            int x = (sign > 0) ? (int)(left-right)-(y-top)*(1/function)*-1 : (int)(left-right)-(y-bottom)*(1/function);
            //printf("(%i | %i)\n", (int)x, y);
            for (int i = -width/2; i <= width/2; i++) {
                frame->pixels[(int)((right+(right-left)+x)+frame->width*(y+i))%(frame->width*frame->height)] = color;
            }
            for (int j = -width/2; j < width/2; j++) {
                frame->pixels[(int)((right+(right-left)+x)+j+frame->width*(y))%(frame->width*frame->height)] = color;
            }   
        } 
    }
    /* Draw circles on end-points */
    draw_center_circle(x1, y1, width, color, frame);
    draw_center_circle(x2, y2, width, color, frame);
}

int getDirection(float x1, float y1, float x2, float y2) {
    if (x1 == x2 || y1 == y2) 
        return 1;

    if (y2 > y1) {
        return (x2 > x1) ? 1 : -1;
    }
    else {
        return (x2 > x1) ? -1 : 1;
    }
}

void IntSpinner(int fixPoint_x, int fixPoint_y, int *spinX, int *spinY, int rad, float time) {

    *spinX = fixPoint_x + cosf(time) * rad;
    *spinY = fixPoint_y + sinf(time) * rad; //+ sinf(time) * rad;
}



void print_pointer(struct frame *frame) {
    printf("========================\n");
    printf("Pointer: 0x%p\n", frame);
    printf("Window Width: %i\n", frame->width);
    printf("Window Height: %i\n", frame->height);
    printf("========================\n");
}
