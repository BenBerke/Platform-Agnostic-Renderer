//
// Created by berke on 8/6/2026.
//

#include "../headers/time.h"
#include "../app.h"

#ifdef _WIN32
ull get_time_ns_win() {
    ull ns;
    GetSystemTimePreciseAsFileTime((FILETIME*)&ns);
    return ns * 100;
}

ull get_time_tick_win() {
    ull t;
    QueryPerformanceCounter((LARGE_INTEGER*)&t);
    return t;
}

#endif

ull g_get_time_ns() {
#ifdef _WIN32
    return get_time_ns_win();
#endif
}

ull g_get_time_tick() {
#ifdef _WIN32
    return get_time_tick_win();
#endif
}