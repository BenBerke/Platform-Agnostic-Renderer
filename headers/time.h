//
// Created by berke on 8/6/2026.
//

#ifndef MINIFB_TIME_H
#define MINIFB_TIME_H

typedef unsigned long long ull;

static ull time_in_ns;

ull g_get_time_ns();
ull g_get_time_tick();

#endif //MINIFB_TIME_H