//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_INPUTMANAGER_H
#define MINIFB_INPUTMANAGER_H

#include "../globals.h"

#define IM_KEY_COUNT 256
#define IM_KEYS_PTR (im_memory)
#define IM_LAST_KEYS_PTR (im_memory + IM_KEY_COUNT)
#define IM_MOUSE_POS_PTR (im_memory + (IM_KEY_COUNT * 2))

#define IM_CURRENT (IM_KEYS_PTR[keycode])
#define IM_PREVIOUS (IM_LAST_KEYS_PTR[keycode])

void im_init();
void im_begin();

bool im_key_get(enum KEYCODES keycode);
bool im_key_get_down(enum KEYCODES keycode);
bool im_key_get_up(enum KEYCODES keycode);

unsigned int im_mouse_pos_x();
unsigned int im_mouse_pos_y();

#endif //MINIFB_INPUTMANAGER_H