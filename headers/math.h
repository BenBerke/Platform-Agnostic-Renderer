//
// Created by berke on 8/6/2026.
//

#ifndef MINIFB_MATH_H
#define MINIFB_MATH_H

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define FLOOR(a) ((float)((int)(a) - ((a) < (int)(a))))

static double inline sqrt(double x);
static float fast_sqrt(float x);

float distance_sqr(int x, int y, int x1, int y1);
float distance_fast(int x, int y, int x1, int y1);
float distance(int x, int y, int x1, int y1);

#endif //MINIFB_MATH_H