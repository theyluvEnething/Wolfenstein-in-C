#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "frame.h"

void clear_screen();
void draw_rectangle(struct frame *frame, int posX, int posY, int width, int height);
void draw_circle(struct frame *frame, int posX, int posY, int rad);
void draw_line(struct frame *frame, int x1, int y1, int x2, int y2);
void print_pointer(struct frame *frame);
static int distance(int x1, int y1, int x2, int y2);
static double clamp(double d, double min, double max);

#endif