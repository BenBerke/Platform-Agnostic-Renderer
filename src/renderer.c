//
// Created by berke on 8/5/2026.
//

#include "../headers/renderer.h"

#include <stdio.h>

void r_init() {
    fenster_open(&r_f);
}

void r_loop() {
    uint32_t t = 0;
    int64_t now = fenster_time();

    while (fenster_loop(&r_f) == 0) {
        for (int i = 0; i < 256; i++) {
            if (r_f.keys[i]) {
                if (i == 'A') printf("A pressed");
            }
        }

        r_clear_screen((char)255, (char)255, (char)255);

        const int64_t time = fenster_time();
        if (time - now < 1000 / 60) fenster_sleep(time - now);
        now = time;
    }
}

void r_destroy() { fenster_close(&r_f); }