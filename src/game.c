//
// Created by berke on 8/5/2026.
//

#include "../headers/declarations.h"

void r_init();
void r_update();

uint32_t r_buffer[W * H];
int keys[256];
struct fenster r_f = {
    .title = "Hello World",
    .width = W,
    .height = H,
    .buf = r_buffer
};

void g_run() {
    r_init();
    r_update();
}