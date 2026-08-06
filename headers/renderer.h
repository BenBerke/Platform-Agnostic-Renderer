//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_RENDERER_H
#define MINIFB_RENDERER_H

#include "../globals.h"

// r at the highest 8, b at the lowest 8
static inline unsigned int rgb_to_int(const unsigned char r, const unsigned char g, const unsigned char b) {return (r << 16) | (g << 8) | b;}

void r_g_init_window(int w, int h, const char* title);
bool r_g_poll_events();
void r_g_update_window();

static inline void r_clear_window(const unsigned char r, const unsigned char g, const unsigned char b) {
#ifdef _WIN32
    __stosd((unsigned long*)screen_buffer, rgb_to_int(r, g, b), W_PIXEL_COUNT);
#endif

}

// ==============
// Draw Functions
// ==============
static inline void r_set_pixel(const unsigned int x, const unsigned int y, const char r, const char g, const char b) {
    screen_buffer[x * y * W_W] = r;
}

void r_draw_line(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, const char r, const char g, const char b);

#endif //MINIFB_RENDERER_H