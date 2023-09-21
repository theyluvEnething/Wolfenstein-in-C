#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include "headers/func.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void print_endl() {
  printf("\n");
}

void print_warning(const char* message) {
    printf(ANSI_COLOR_YELLOW "[WARNING] " ANSI_COLOR_RESET);
    for (int i = 0; message[i] != '\0'; i++) {
        printf("%c", message[i]);
    }
}

void print_error(const char* message) {
    printf(ANSI_COLOR_RED "[ERROR] " ANSI_COLOR_RESET);
    for (int i = 0; message[i] != '\0'; i++) {
      printf("%c", message[i]);
    }
}


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

int int_min(float a, float b) {
  return (a < b) ? a : b;
}

int int_max(float a, float b) {
  return (a > b) ? a : b;
}