//
// Created by berke on 5/20/2026.
//

#ifndef SOFTWARERENDERER_VECTOR3_H
#define SOFTWARERENDERER_VECTOR3_H

#include <immintrin.h>

typedef union Vector3 {
    // Keep this first so aggregate initialization such as {x, y, z}
    // initializes the unused SIMD lane to zero.
    float values[4];

    struct {
        float x;
        float y;
        float z;
        float _padding;
    };

    __m128 simd;
} Vector3;

static inline Vector3 vector3_create(const float x, const float y, const float z) {
    return (Vector3){.values = {x, y, z, 0.0f}};
}

float vector3_length(Vector3 vector);
float vector3_distance(Vector3 vector1, Vector3 vector2);
Vector3 vector3_add(Vector3 vector1, Vector3 vector2);
Vector3 vector3_subtract(Vector3 vector1, Vector3 vector2);
Vector3 vector3_normalized(Vector3 vector);
Vector3 vector3_multiply_f(Vector3 vector, float scalar);

#if defined(__cplusplus)
static_assert(sizeof(Vector3) == 16, "Vector3 must occupy one SIMD register");
#else
_Static_assert(sizeof(Vector3) == 16, "Vector3 must occupy one SIMD register");
#endif

#endif // SOFTWARERENDERER_VECTOR3_H