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

        r_clear_window(r, im_mouse_pos_x(), im_mouse_pos_y());

        r_g_update_window();
    }
}