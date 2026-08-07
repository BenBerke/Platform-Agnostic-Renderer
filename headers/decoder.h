//
// Created by berke on 8/6/2026.
//

#ifndef MINIFB_DECODER_H
#define MINIFB_DECODER_H

#include "../headers/typedefs.h"

u32* png_to_bitstream(const char* filepath, int* out_width, int* out_height);

#endif //MINIFB_DECODER_H