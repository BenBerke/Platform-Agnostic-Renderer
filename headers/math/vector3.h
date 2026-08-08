//
// Created by berke on 8/7/2026.
//

#ifndef MINIFB_VECTOR3_H
#define MINIFB_VECTOR3_H

typedef struct Vector3 {
    float x, y, z, w;
} Vector3;

float v3_length(Vector3 vector);
float v3_length_squared(Vector3 vector);
float v3_distance(Vector3 vector1, Vector3 vector2);
Vector3 v3_add(Vector3 vector1, Vector3 vector2);
Vector3 v3_subtract(Vector3 vector1, Vector3 vector2);
Vector3 v3_normalized(Vector3 vector);
Vector3 v3_multiply_f(Vector3 vector, float scalar);

#endif //MINIFB_VECTOR3_H