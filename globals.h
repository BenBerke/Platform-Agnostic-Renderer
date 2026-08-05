//
// Created by berke on 8/5/2026.
//
#ifndef MINIFB_CONFIG_H
#define MINIFB_CONFIG_H

// Holds information that is generally required by majority of the files

#include "app.h"

#define W_W 960
#define W_H 960

#define bool char
#define true 1
#define false 0

extern DWORD screen_buffer[W_W * W_H];

#define SB_SIZE sizeof(screen_buffer)

enum KEYCODES {
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

    KC_ESC = 27,
};

#endif //MINIFB_CONFIG_H