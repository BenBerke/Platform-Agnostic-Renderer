//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_INPUTMANAGER_H
#define MINIFB_INPUTMANAGER_H

#include "../globals.h"

#define IM_KEY_COUNT 256
#define IM_KEYS_PTR (im_memory)
#define IM_LAST_KEYS_PTR (im_memory + IM_KEY_COUNT)

#define IM_CURRENT *(IM_KEYS_PTR + keycode)
#define IM_PREVIOUS *(IM_LAST_KEYS_PTR + keycode)

void im_init();

void im_begin();

bool im_get_key(enum KEYCODES keycode);
bool im_get_key_down(enum KEYCODES keycode);
bool im_get_key_up(enum KEYCODES keycode);

#endif //MINIFB_INPUTMANAGER_H