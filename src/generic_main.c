//
// Created by berke on 8/5/2026.
//
#include "../headers/renderer.h"
#include "../headers/input_manager.h"
#include "../headers/debug.h"

void generic_main() {
    r_g_init_window(960, 800, "Hello world");
    im_init();
    int r = 0;

    bool running = true;
    while (running) {
        im_begin();

        running = r_g_poll_events();

        if (im_key_get(KC_A)) r++;

        r_set_draw_color(255, 255, 255);
        r_clear_window();

        r_set_draw_color(0, 0, 0);
        r_draw_line(10, 10, 500, 850);

        r_g_update_window();
    }
}