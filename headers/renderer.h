//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_RENDERER_H
#define MINIFB_RENDERER_H

#include <stdint.h>
#include "../fenster.h"

#define W 420
#define H 540

static uint32_t buffer[W * H];
static struct fenster f = {
    .title = "Hello World",
    .width = W,
    .height = H,
    .buf = buffer,
};

inline void r_init();
// r is the highest bits, b is the lowest
inline uint32_t r_rgb_to_i32(char r, char g, char b);
inline void r_clear_screen(char r, char g, char b);
inline void r_set_pixel(int x, int y, char r, char g, char b);

#endif //MINIFB_RENDERER_H