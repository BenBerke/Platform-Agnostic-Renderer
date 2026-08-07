//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_RENDERER_H
#define MINIFB_RENDERER_H

#include "../globals.h"
#include "../headers/typedefs.h"

static int draw_color;

// r at the highest 8, b at the lowest 8
static inline int r_rgb_to_int(const unsigned char r, const unsigned char g, const unsigned char b) {return (r << 24) | (g << 16) | (b << 8);}

static inline void r_int_to_rgb(const unsigned int i, unsigned char* r, unsigned char* g, unsigned char* b) {
    *r = (i & 0xFF000000) >> 24;
    *g = (i & 0x00FF0000) >> 16;
    *b = (i & 0x0000FF00) >> 8;
}
static inline int cord_to_index(const int x, const int y) {return x + y * W_W;}

static inline void r_set_draw_color(const unsigned char r, const unsigned char g, const unsigned char b) {
    draw_color = r_rgb_to_int(r, g, b);
}
static inline void r_set_draw_color_int(const int color) {
    draw_color = color;
}

void r_g_init_window(int w, int h, const char* title);
bool r_g_poll_events();
void r_g_update_window();

static inline void r_clear_window() {
#ifdef _WIN32
    __stosd((unsigned long*)screen_buffer, draw_color, W_PIXEL_COUNT);
#endif

}

// ==============
// Draw Functions
// ==============
static inline void r_set_pixel(const int x, const int y) {
    if (x < 0 || x >= W_W || y < 0 || y >= W_H) return;
    screen_buffer[cord_to_index(x, y)] = draw_color;
}

void r_draw_line(int x, int y, int x1, int y1);
void r_draw_horizontal_line(int x, int x1, int y);
void r_draw_fill_rect(int x, int y, int w, int h);
void r_draw_fill_circle(int x, int y, int half_r);
void r_draw_texture_scaled(const Texture* texture, int x, int y, float scale);
void r_draw_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void r_draw_texture_triangle(
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2,
    int x3, int y3, float u3, float v3,
    const Texture* texture);

#endif //MINIFB_RENDERER_H