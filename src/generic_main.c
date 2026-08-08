//
// Created by berke on 8/5/2026.
//
#include "../headers/renderer.h"
#include "../headers/input_manager.h"
#include "../headers/debug.h"
#include "../headers/time.h"
#include "../headers/decoder.h"

void generic_main() {
    r_g_init_window(W_W, W_H, "Test");
    im_init();

    const Texture pb = png_to_bitstream("C:/Users/berke/Pictures/Screenshots/Screenshot 2026-08-01 224134.png");
    if (!pb.data) {
        g_print("Picture couldn't work");
        return;
    }

    bool running = true;
    while (running) {
        im_begin();

        running = r_g_poll_events();

        r_set_draw_color(255, 255, 255);
        r_clear_window();

        r_set_draw_color(0, 0, 0);

        r_draw_texture_triangle(
            0, 0, 0, 0, 0,
            pb.h, 0.0f, 1.0f,
            pb.w, pb.h, 1.0f, 1.0f,
            &pb
            );
        r_draw_texture_triangle(
             pb.w, 0, 1.0f, 0,
             0, 0, 0, 0,
             pb.w, pb.h, 1.0f, 1.0f,
             &pb
        );

        r_g_update_window();
    }
}