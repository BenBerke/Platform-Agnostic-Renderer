#include "headers/renderer.h"

uint32_t r_buffer[W * H];
int keys[256];
struct fenster r_f = {
    .title = "Hello World",
    .width = W,
    .height = H,
    .buf = r_buffer
};

int main() {
    r_init();
    r_loop();
    r_destroy();
    return 0;
}
