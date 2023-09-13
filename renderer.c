#include <stdbool.h>
#include<math.h>

#include "renderer.h"



void clear_screen() {
    printf("Clearing Screen.\n");

    // uint32_t *pixel = (uint32_t * )memory; 

    // for (int pixel_number = 0; pixel_number < width*height; pixel_number++)
    // {
    //     *pixel++ = color;
    // }
}

void draw_rectangle(struct frame *frame, int posX, int posY, int width, int height) {
    for (int y=posY;y<=posY+height;y++) {
        for (int x=posX;x<=posX+width;x++) {
            frame->pixels[(x+frame->width*y)%(frame->height*frame->width)] = 0xAAAAFF;
        }
    }
}

void draw_circle(struct frame *frame, int posX, int posY, int rad) {
    for (int y=posY;y<posY+rad;y++) {
        for (int x=posX;x<posX+rad;x++) {

            if (distance(x, y, posX+rad/2, posY+rad/2) >= rad/2) 
                continue;

            frame->pixels[(x+frame->width*y)%(frame->width*frame->height)] = 0xFFFFF;
        }
    }
}

void draw_line(struct frame *frame, int x1, int y1, int x2, int y2) {
    float function = ((float)(y2-y1))/((float)(x2-x1));
    printf("%f\n", function);

    int width = 10;

    for (float x=x1; x<x2; x++) {
        int y = (int)x*function;


        for (int i = 0; i < width; i++) {
            frame->pixels[(int)(x+frame->width*(y+i))%(frame->width*frame->height)] = 0xFFFFFF;
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


void print_pointer(struct frame *frame) {
    printf("========================\n");
    printf("Pointer: 0x%p\n", frame);
    printf("Window Width: %i\n", frame->width);
    printf("Window Height: %i\n", frame->height);
    printf("========================\n");
}

static int distance(int x1, int y1, int x2, int y2) {
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

double clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;
}
