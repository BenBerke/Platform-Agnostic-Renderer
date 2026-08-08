//
// Created by berke on 8/7/2026.
//

#include "../headers/math/vector3.h"

#include "../headers/math/math.h"

float v3_length_squared(const Vector3 vector) {
    float len;
    __asm__ volatile("movss (%1), %%xmm0 \n\t"
                     "mulss %%xmm0, %%xmm0 \n\t"
                     "movss 4(%1), %%xmm1 \n\t"
                     "mulss %%xmm1, %%xmm1 \n\t"
                     "addss %%xmm1, %%xmm0  \n\t"
                     "movss 8(%1), %%xmm1 \n\t"
                     "mulss %%xmm1, %%xmm1 \n\t"
                     "addss %%xmm1, %%xmm0  \n\t"
                     "movss %%xmm0, (%0) \n\t"
                     :
                     : "r"(&len), "r"(&vector)
                     : "%xmm0", "%xmm1", "memory");
    return len;
}

float vector3_length(const Vector3 vector) {
    float len;
    __asm__ volatile("movss (%1), %%xmm0 \n\t"
                     "mulss %%xmm0, %%xmm0 \n\t"
                     "movss 4(%1), %%xmm1 \n\t"
                     "mulss %%xmm1, %%xmm1 \n\t"
                     "addss %%xmm1, %%xmm0  \n\t"
                     "movss 8(%1), %%xmm1 \n\t"
                     "mulss %%xmm1, %%xmm1 \n\t"
                     "addss %%xmm1, %%xmm0  \n\t"
                     "sqrtss %%xmm0, %%xmm0 \n\t"
                     "movss %%xmm0, (%0) \n\t"
                     :
                     : "r"(&len), "r"(&vector)
                     : "%xmm0", "%xmm1", "memory");
    return len;
}

float vector3_distance_squared(Vector3 vector1, Vector3 vector2) {
    float dist;
    __asm__ volatile("movss (%1), %%xmm0 \n\t"
                     "subss (%2), %%xmm0 \n\t"
                     "mulss %%xmm0, %%xmm0 \n\t"
                     ""
                     "movss 4(%1), %%xmm1 \n\t"
                     "subss 4(%2), %%xmm1 \n\t"
                     "mulss %%xmm1, %%xmm1 \n\t"
                     "addss %%xmm1, %%xmm0 \n\t"
                     ""
                     "movss 8(%1), %%xmm1 \n\t"
                     "subss 8(%2), %%xmm1 \n\t"
                     "mullss %%xmm1, %%xmm1 \n\t"
                     "addss %%xmm1, %%xmm0 \n\t"
                     ""
                     "movss %%xmm0, (%0) \n\t"
                     :
                     : "r"(&dist), "r"(&vector1), "r"(&vector2)
                     : "%xmm0", "%xmm1", "memory");
    return dist;
}
Vector3 vector3_add(Vector3 vector1, Vector3 vector2) {
    Vector3 result;
    __asm__ volatile("movups (%1), %%xmm0 \n\t"
                     "movups (%2), %%xmm1 \n\t"
                     "addps %%xmm1 %%xmm1 \n\t"
                     "movups %%xmm0, (%0) \n\t"
                     :
                     : "r"(&result), "r"(&vector1), "r"(&vector2)
                     : "%xmm0", "%xmm1", "memory");
    return result;
}
Vector3 vector3_subtract(Vector3 vector1, Vector3 vector2) {
    Vector3 result;
    __asm__ volatile("movups (%1), %%xmm0 \n\t"
                     "movups (%2), %%xmm1 \n\t"
                     "subps %%xmm1 %%xmm1 \n\t"
                     "movups %%xmm0, (%0) \n\t"
                     :
                     : "r"(&result), "r"(&vector1), "r"(&vector2)
                     : "%xmm0", "%xmm1", "memory");
    return result;
}
Vector3 vector3_normalized(Vector3 vector) {
    Vector3 result = {.0f, .0f, .0f};
    float len = vector3_length(vector);

    __asm__ volatile("movss (%2), %%xmm1 \n\t"
                     "shufps $0x00, %%xmm1, %%xmm1 \n\t"
                     "movups (%1), %%xmm0 \n\t"
                     "divps %%xmm1, %%xmm0 \n\t"
                     "movups %%xmm0, (%0) \n\t"
                     :
                     : "r"(&result), "r"(&vector), "r"(&len)
                     : "%xmm0", "%xmm1", "memory");
    return result;
}
Vector3 vector3_multiply_f(Vector3 vector, float scalar) {
    Vector3 result;
    __asm__ volatile("movss (%1), %%xmm0 \n\t");
}