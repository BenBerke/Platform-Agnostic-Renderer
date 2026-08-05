#include "headers/renderer.h"

int main() {
    r_init();

    uint32_t t = 0;
    int64_t now = fenster_time();

    while (fenster_loop(&f) == 0) {
        r_clear_screen((char)255, (char)255, (char)255);

        int64_t time = fenster_time();
        if (time - now < 1000 / 60) fenster_sleep(time - now);
        now = time;
    }

    fenster_close(&f);
    return 0;
}
