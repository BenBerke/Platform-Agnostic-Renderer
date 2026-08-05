//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_RENDERER_H
#define MINIFB_RENDERER_H

#include "declarations.h"

enum KEY_CODES {
    KC_0 = '0',
    KC_1 = '1',
    KC_2 = '2',
    KC_3 = '3',
    KC_4 = '4',
    KC_5 = '5',
    KC_6 = '6',
    KC_7 = '7',
    KC_8 = '8',
    KC_9 = '9',
    KC_A = 'A',
    KC_B = 'B',
    KC_C = 'C',
    KC_D = 'D',
    KC_E = 'E',
    KC_F = 'F',
    KC_G = 'G',
    KC_H = 'H',
    KC_I = 'I',
    KC_J = 'J',
    KC_K = 'K',
    KC_L = 'L',
    KC_M = 'M',
    KC_N = 'N',
    KC_O = 'O',
    KC_P = 'P',
    KC_Q = 'Q',
    KC_R = 'R',
    KC_S = 'S',
    KC_T = 'T',
    KC_U = 'U',
    KC_V = 'V',
    KC_W = 'W',
    KC_X = 'X',
    KC_Y = 'Y',
    KC_Z = 'Z',
};

void r_init();
void r_loop();
void r_destroy();

static inline uint32_t r_rgb_to_i32(const char r, const char g, const char b) {
    uint32_t value = 0;
    value |= r << 16; value |= g << 8; value |= b;
    return value;
}
static inline void r_clear_screen(const char r, const char g, const char b) {
    const uint32_t color = r_rgb_to_i32(r, g, b);
    memset(r_buffer, (int)color, W * H * sizeof(uint32_t));
}
static inline void r_set_pixel(const int x, const int y, const char r, const char g, const char b) {
    const uint32_t color = r_rgb_to_i32(r, g, b);
    r_buffer[(x+1) * (y+1) - 1] = color;
}

#endif //MINIFB_RENDERER_H