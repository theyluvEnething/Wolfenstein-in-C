#include <stdbool.h>
#include <math.h>
#include "renderer.h"
#include "func.h"

int getDirection(float x1, float y1, float x2, float y2);

void clear_screen() {
    printf("Clearing Screen.\n");

    // uint32_t *pixel = (uint32_t * )memory; 

    // for (int pixel_number = 0; pixel_number < width*height; pixel_number++)
    // {
    //     *pixel++ = color;
    // }
}

void draw_rectangle(struct frame *frame, int posX, int posY, int width, int height) {


    int left = (width >= 0) ? posX : posX + width;
    int right = (width >= 0) ? posX + width : posX;
    int bottom = (height >= 0) ? posY : posY + height;
    int top = (height >= 0) ? posY + height : posY;

    // printf("Height: %i\n", height);
    // printf("Left: %i\n", left);
    // printf("Right: %i\n", right);
    // printf("Bottom: %i\n", bottom);
    // printf("Top: %i\n\n", top);

    for (int y=bottom;y<=top;y++) {
        for (int x=left;x<=right;x++) {
            frame->pixels[(x+frame->width*y)%(frame->height*frame->width)] = 0xAAAAFF;
        }
    }
}

void draw_circle(struct frame *frame, int posX, int posY, int rad) {
    for (int y=posY;y<posY+rad;y++) {
        for (int x=posX;x<posX+rad;x++) {

            if (int_distance(x, y, posX+rad/2, posY+rad/2) >= rad/2) 
                continue;

            frame->pixels[(x+frame->width*y)%(frame->width*frame->height)] = 0xFFFFF;
        }
    }
}

void draw_line(struct frame *frame, int x1, int y1, int x2, int y2) {

    int width = 10;
    int left = min(x1, x2);
    int right = max(x1, x2);
    int bottom = min(y1, y2);
    int top = max(y1, y2);
    float function = (float)(top-bottom)/(right-left);
    int sign = getDirection(x1, y1, x2, y2);

    draw_rectangle(frame, left, bottom, right-left, top-bottom);
    draw_circle(frame, x1, y1, 20);
    draw_circle(frame, x2, y2, 20);

    // printf("Left: %i\n", left);
    // printf("Right: %i\n", right);
    // printf("Bottom: %i\n", bottom);
    // printf("Top: %i\n", top);
    // printf("Sign: %i\n", sign);
    // printf("Function: %f\n\n", function);
    

    for (float x=left; x<right; x++) {

        int y = (sign > 0) ? (int)(x-left)*function : (int)(top-bottom)-(x-left)*function;
        //printf("(%i | %i)\n", (int)x, y);
        for (int i = 0; i < width; i++) {
            frame->pixels[(int)(x+frame->width*(y+i+bottom))%(frame->width*frame->height)] = 0xFFFFFF;
        }
    }
    // for (int x=x1; x<x2; x++) {
    //     //printf("%i\n", x);
    //     for (int y=y1; y<y2; y++) {
    //         float calc = x * function;
    //         //printf("%f\n", calc);
    //         if (y-3 < calc && calc < y+3) frame->pixels[(x+frame->width*y)%(frame->width*frame->height)] = 0xFFFFFF;
    //     }
    // }
}

int getDirection(float x1, float y1, float x2, float y2) {
    if (x1 == x2 || y1 == y2) 
        return 0;

    if (y2 > y1) {
        return (x2 > x1) ? 1 : -1;
    }
    else {
        return (x2 > x1) ? -1 : 1;
    }
}

void IntSpinner(int fixPoint_x, int fixPoint_y, int *spinX, int *spinY, int rad, float time) {

    *spinX = fixPoint_x + cosf(time) * rad;
    printf("spinX: %i\n", *spinX);
    *spinY = fixPoint_y + sinf(time) * rad; //+ sinf(time) * rad;
    printf("spinY: %i\n", *spinX);

}



void print_pointer(struct frame *frame) {
    printf("========================\n");
    printf("Pointer: 0x%p\n", frame);
    printf("Window Width: %i\n", frame->width);
    printf("Window Height: %i\n", frame->height);
    printf("========================\n");
}
