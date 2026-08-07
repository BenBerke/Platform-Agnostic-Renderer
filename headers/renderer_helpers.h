//
// Created by berke on 8/7/2026.
//

#ifndef MINIFB_RENDERER_HELPERS_H
#define MINIFB_RENDERER_HELPERS_H

#include "../globals.h"
typedef int Color;

static inline float edge_side(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3) {
    return (x3 - x2) * (y1 - y2) - (x1 - x2) * (y3 - y2);
}

static inline bool helper_r_point_in_triangle(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float px, const float py) {
    const float d1 = edge_side(x1, y1, x2, y2, px, py);
    const float d2 = edge_side(x2, y2, x3, y3, px, py);
    const float d3 = edge_side(x3, y3, x1, y1, px, py);

    const bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    const bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return (bool)!(has_neg && has_pos);
}

static inline Color helper_r_get_texture_pixel
    (const int x1, const int y1, const float u1, const float v1,
    const int x2, const int y2, const float u2, const float v2,
    const int x3, const int y3, const float u3, const float v3,
    const int px, const int py, const Texture* texture) {

    const int total_area = (int)edge_side((float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3);
    if (total_area == 0) return 0;

    const float w1 = edge_side((float)x2, (float)y2, (float)x3, (float)y3, (float)px, (float)py) / (float)total_area;
    const float w2 = edge_side((float)x3, (float)y3, (float)x1, (float)y1, (float)px, (float)py) / (float)total_area;
    const float w3 = edge_side((float)x1, (float)y1, (float)x2, (float)y2, (float)px, (float)py) / (float)total_area;

    const float u = (w1 * u1) + (w2 * u2) + (w3 * u3);
    const float v = (w1 * v1) + (w2 * v2) + (w3 * v3);

    const int tex_x = MAX(0, (int)(u * (texture->w - 1)));
    const int tex_y = MAX(0, (int)(v * (texture->h - 1)));

    const u32 index = (tex_y * texture->w) + tex_x;
    const Color color = texture->data[index];

    return color;
}

#endif //MINIFB_RENDERER_HELPERS_H