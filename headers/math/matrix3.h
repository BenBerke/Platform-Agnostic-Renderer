//
// Created by berke on 8/7/2026.
//

#ifndef MINIFB_MATRIX3_H
#define MINIFB_MATRIX3_H

#include "vector3.h"

typedef struct Matrix3 {
    float m[3][3];
} Matrix3;

Matrix3 matrix3_multiply(const Matrix3* m, const Matrix3* n);
Matrix3 matrix3_add(const Matrix3* m, const Matrix3* n);
Matrix3 matrix3_subtract(const Matrix3* m, const Matrix3* n);
Vector3 matrix3_multiply_vector3(const Matrix3* m, Vector3 v);


#endif //MINIFB_MATRIX3_H