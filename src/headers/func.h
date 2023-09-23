#ifndef FUNC_H
#define FUNC_H

#include <stdarg.h>

void print_endl();
void print_warning(const char* message, ...);
void print_error(const char* message, ...);
void print_info(const char* message, ...);

float float_distance(int x1, int y1, int x2, int y2);
float float_clamp(float d, float min, float max);
float float_abs(float x);

int int_distance(int x1, int y1, int x2, int y2);
int int_clamp(float d, float min, float max);
int int_abs(float x);
int int_min(float a, float b);
int int_max(float a, float b);

#endif