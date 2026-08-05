//
// Created by berke on 3/11/2026.
//

#ifndef VECTOR2_H
#define VECTOR2_H

#include <immintrin.h>

typedef union Vector2 {
    // Keep this first so aggregate initialization such as {x, y}
    // initializes the remaining SIMD lanes to zero.
    float values[4];

    struct {
        float x;
        float y;
        float _padding0;
        float _padding1;
    };

    __m128 simd;
} Vector2;

static inline Vector2 vector2_create(const float x, const float y) {
    return (Vector2){.values = {x, y, 0.0f, 0.0f}};
}

float vector2_length(Vector2 vector);
float vector2_distance(Vector2 vector1, Vector2 vector2);
Vector2 vector2_add(Vector2 vector1, Vector2 vector2);
Vector2 vector2_subtract(Vector2 vector1, Vector2 vector2);
Vector2 vector2_normalized(Vector2 vector);
Vector2 vector2_multiply_with_float(Vector2 vector, float scalar);
void vector2_swap(Vector2* a, Vector2* b);

#if defined(__cplusplus)
static_assert(sizeof(Vector2) == 16, "Vector2 must occupy one SIMD register");
#else
_Static_assert(sizeof(Vector2) == 16, "Vector2 must occupy one SIMD register");
#endif

#endif // VECTOR2_H