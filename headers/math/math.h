//
// Created by berke on 8/6/2026.
//

#ifndef MINIFB_MATH_H
#define MINIFB_MATH_H

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define FLOOR(a) ((float)((int)(a) - ((a) < (int)(a))))

static double m_sqrt(double x);
static float m_sqrt_fast(float x);

float m_distance_sqr(int x, int y, int x1, int y1);
float m_distance_fast(int x, int y, int x1, int y1);
float m_distance(int x, int y, int x1, int y1);

#endif //MINIFB_MATH_H