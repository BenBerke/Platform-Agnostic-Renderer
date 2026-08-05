//
// Created by berke on 5/20/2026.
//

#include "../../headers/math/Matrix3.h"

static inline __m128 splat_x(const __m128 value) {
    return _mm_shuffle_ps(value, value, _MM_SHUFFLE(0, 0, 0, 0));
}

static inline __m128 splat_y(const __m128 value) {
    return _mm_shuffle_ps(value, value, _MM_SHUFFLE(1, 1, 1, 1));
}

static inline __m128 splat_z(const __m128 value) {
    return _mm_shuffle_ps(value, value, _MM_SHUFFLE(2, 2, 2, 2));
}

Matrix3 matrix3_multiply(const Matrix3* m, const Matrix3* n) {
    Matrix3 result;

    const __m128 m0 = m->simd[0];
    const __m128 m1 = m->simd[1];
    const __m128 m2 = m->simd[2];

    for (int column = 0; column < 3; column++) {
        const __m128 nColumn = n->simd[column];

        result.simd[column] = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(m0, splat_x(nColumn)),
                _mm_mul_ps(m1, splat_y(nColumn))
            ),
            _mm_mul_ps(m2, splat_z(nColumn))
        );
    }

    return result;
}

Matrix3 matrix3_add(const Matrix3* m, const Matrix3* n) {
    Matrix3 result;

    result.simd[0] = _mm_add_ps(m->simd[0], n->simd[0]);
    result.simd[1] = _mm_add_ps(m->simd[1], n->simd[1]);
    result.simd[2] = _mm_add_ps(m->simd[2], n->simd[2]);

    return result;
}

Matrix3 matrix3_subtract(const Matrix3* m, const Matrix3* n) {
    Matrix3 result;

    result.simd[0] = _mm_sub_ps(m->simd[0], n->simd[0]);
    result.simd[1] = _mm_sub_ps(m->simd[1], n->simd[1]);
    result.simd[2] = _mm_sub_ps(m->simd[2], n->simd[2]);

    return result;
}

Vector3 matrix3_multiply_vector3(const Matrix3* m, const Vector3 v) {
    Vector3 result;

    result.simd = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(m->simd[0], splat_x(v.simd)),
            _mm_mul_ps(m->simd[1], splat_y(v.simd))
        ),
        _mm_mul_ps(m->simd[2], splat_z(v.simd))
    );

    return result;
}