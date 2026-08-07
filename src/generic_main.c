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

    const Texture pic_buffer = png_to_bitstream("C:/Users/berke/Pictures/Screenshots/Screenshot 2026-08-01 224134.png");
    if (!pic_buffer.data) {
        g_print("Picture couldn't work");
        return;
    }

    bool running = true;
    while (running) {
        im_begin();

        running = r_g_poll_events();

        r_set_draw_color(255, 255, 255);
        r_clear_window();

        r_draw_texture_scaled(&pic_buffer, 100, 100, 1.3f);

        r_g_update_window();
    }
}