//
// Created by berke on 8/5/2026.
//
#include "../headers/renderer.h"
#include "../headers/input_manager.h"
#include "../headers/debug.h"
#include "../headers/time.h"

void generic_main() {
    r_g_init_window(960, 800, "Hello world");
    im_init();

    float x = 0, r = 30;
    float y = W_H-r;

    float velocity_y = 0;
    float jump_impulse = 15.0f;
    float gravity = .03f;
    float speed = 30.0f;

    unsigned long long last_time = g_get_time_tick();
    double dt = 0;

    bool running = true;
    while (running) {
        unsigned long long cur_time = g_get_time_tick();
        dt = (double)(cur_time - last_time) / 1000000.0;
        last_time = cur_time;

        im_begin();

        running = r_g_poll_events();

        if (im_key_get(KC_A)) x -= speed * dt;
        if (im_key_get(KC_D)) x += speed * dt;

        bool is_grounded = (y >= W_H - r);
        if (im_key_get_down(KC_SPACE) && is_grounded) velocity_y = -jump_impulse;

        if (!is_grounded) velocity_y += gravity;

        y += velocity_y * dt;

        if (y >= W_H - r) {
            y = W_H - r;
            velocity_y = 0.0f;
        }
        if (y <= 0) {
            y = 0;
            velocity_y = 0.0f;
        }

        r_set_draw_color(255, 255, 255);
        r_clear_window();

        r_set_draw_color(0, 0, 0);
        r_draw_fill_circle((int)x, (int)y, (int)r);

        r_g_update_window();
    }
}