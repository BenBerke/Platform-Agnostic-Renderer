//
// Created by berke on 8/5/2026.
//
#include "../headers/renderer.h"
#include "../headers/input_manager.h"
#include "../headers/debug.h"
#include "../headers/time.h"
#include "../headers/decoder.h"

void generic_main() {
    r_g_init_window(W_W, W_H, "I love you");
    im_init();

    int w = 0, h = 0;
    const u32* pic_buffer = png_to_bitstream("C:/Users/berke/Pictures/Screenshots/Screenshot 2026-08-01 224134.png", &w, &h);
    if (!pic_buffer) {
        g_print("Picture couldn't work");
        return;
    }

    bool running = true;
    while (running) {
        im_begin();

        running = r_g_poll_events();

        r_set_draw_color(255, 255, 255);
        r_clear_window();

        for (int ay = 0; ay < h; ay++) {
            for (int ax = 0; ax < w; ax++) {
                u8 ar, ag, ab;
                r_int_to_rgb(*(pic_buffer + ax + ay * w), &ar, &ag, &ab);

                r_set_draw_color(ar, ag, ab);
                r_set_pixel(ax, ay);
            }
        }

        r_g_update_window();
    }
}