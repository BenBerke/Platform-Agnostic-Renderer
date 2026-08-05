//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_DECLARATIONS_H
#define MINIFB_DECLARATIONS_H

#include <stdint.h>
#include "../fenster.h"

#define W 420
#define H 540

extern uint32_t r_buffer[W * H];
extern int keys[256];
extern  struct fenster r_f;

#endif //MINIFB_DECLARATIONS_H