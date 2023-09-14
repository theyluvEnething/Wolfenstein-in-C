#include <math.h>
#include "func.h"
#include <stdio.h>

/* float */
float float_abs(float x) {
    if (x >= 0) return x;
    else return -x;
}
float float_distance(int x1, int y1, int x2, int y2) {
    return (float)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}
float float_clamp(float d, float min, float max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}


/* INT */
int int_abs(float x) {
    if (x >= 0) return (int)x;
    else return (int)-x;
}

int int_distance(int x1, int y1, int x2, int y2) {
    return (int)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

int int_clamp(float d, float min, float max) {
  const float t = (d < min) ? min : d;
  return (int)(t > max) ? max : t;
}

int min(float a, float b) {
  return (a < b) ? a : b;
}

int max(float a, float b) {
  return (a > b) ? a : b;
}