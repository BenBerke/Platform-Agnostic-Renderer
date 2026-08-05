//
// Created by berke on 3/11/2026.
//

#include "../Headers/Vector2.h"

static inline float vector2_length_squared_simd(const __m128 vector) {
    const __m128 squared = _mm_mul_ps(vector, vector);
    const __m128 ySquared = _mm_shuffle_ps(squared, squared, _MM_SHUFFLE(1, 1, 1, 1));
    return _mm_cvtss_f32(_mm_add_ss(squared, ySquared));
}

float vector2_length(const Vector2 vector) {
    const __m128 lengthSquared = _mm_set_ss(vector2_length_squared_simd(vector.simd));
    return _mm_cvtss_f32(_mm_sqrt_ss(lengthSquared));
}

float vector2_distance(const Vector2 vector1, const Vector2 vector2) {
    const __m128 difference = _mm_sub_ps(vector1.simd, vector2.simd);
    const __m128 distanceSquared = _mm_set_ss(vector2_length_squared_simd(difference));
    return _mm_cvtss_f32(_mm_sqrt_ss(distanceSquared));
}

Vector2 vector2_add(const Vector2 vector1, const Vector2 vector2) {
    Vector2 result;
    result.simd = _mm_add_ps(vector1.simd, vector2.simd);
    return result;
}

Vector2 vector2_subtract(const Vector2 vector1, const Vector2 vector2) {
    Vector2 result;
    result.simd = _mm_sub_ps(vector1.simd, vector2.simd);
    return result;
}

Vector2 vector2_normalized(const Vector2 vector) {
    const float lengthSquared = vector2_length_squared_simd(vector.simd);

    if (lengthSquared <= 0.0f) {
        return vector2_create(0.0f, 0.0f);
    }

    const __m128 length = _mm_sqrt_ps(_mm_set1_ps(lengthSquared));

    Vector2 result;
    result.simd = _mm_div_ps(vector.simd, length);
    return result;
}

Vector2 vector2_multiply_with_float(const Vector2 vector, const float scalar) {
    Vector2 result;
    result.simd = _mm_mul_ps(vector.simd, _mm_set1_ps(scalar));
    return result;
}

void vector2_swap(Vector2* a, Vector2* b) {
    const __m128 temporary = a->simd;
    a->simd = b->simd;
    b->simd = temporary;
}