//
// Created by berke on 8/5/2026.
//

#include "../headers/renderer.h"

inline void r_init() {
    fenster_open(&f);
}

inline uint32_t r_rgb_to_i32(const char r, const char b, const char g) {
    uint32_t value = 0;
    value |= r << 16; value |= g << 8; value |= b;
    return value;
}

inline void r_clear_screen(const char r, const char g, const char b) {
    const uint32_t color = r_rgb_to_i32(r, g, b);
    memset(buffer, (int)color, W * H * sizeof(uint32_t));
}
inline void r_set_pixel(const int x, const int y, const char r, const char g, const char b) {
    const uint32_t color = r_rgb_to_i32(r, g, b);
    buffer[(x+1) * (y+1) - 1] = color;
}