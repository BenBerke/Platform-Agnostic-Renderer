#ifndef MATRIX3_H
#define MATRIX3_H

#include <immintrin.h>

#include "Vector3.h"

typedef union Matrix3 {
    // m[column][row]
    float m[3][4];

    __m128 simd[3];
} Matrix3;

Matrix3 matrix3_multiply(const Matrix3* m, const Matrix3* n);
Matrix3 matrix3_add(const Matrix3* m, const Matrix3* n);
Matrix3 matrix3_subtract(const Matrix3* m, const Matrix3* n);
Vector3 matrix3_multiply_vector3(const Matrix3* m, Vector3 v);

#endif