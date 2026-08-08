//
// Created by berke on 8/6/2026.
//

#include "../headers/math/math.h"

float m_sqrt_fast(float x) {
    if (x < 0.0f) return 0.0f;

    int i = 0;
    const float x_half = x * .5f;

    i = *(int*)&x;
    i = 0x5F3759DF - (i >> 1);

    x = *(float*)&i;

    x = x * (1.5f - x_half * x * x); // inverse square root

    return x * x;
}

double m_sqrt(const double x) {
    if (x < 0.0) return 0.0;
    if (x == 0.0 || x == 1.0) return x;

    double guess = x / 2.0;
    double prev_guess = .0;

    while (guess != prev_guess) {
        prev_guess = guess;
        guess = (guess + x / guess) / 2.0;
    }

    return guess;
}

float m_distance_sqr(int x, int y, int x1, int y1) {
    const int a = x1-x;
    const int b = y1-y;

    return (float)(a*a + b*b);
}

float m_distance_fast(int x, int y, int x1, int y1) {
    return m_sqrt_fast(m_distance_sqr(x, y, x1, y1));
}

float m_distance(int x, int y, int x1, int y1) {
    return m_sqrt(m_distance_sqr(x, y, x1, y1));
}