//
// Created by berke on 8/5/2026.
//

#include "../headers/input_manager.h"
#include "../app.h"
#include "../globals.h"
#include "../headers/debug.h"
#include "../headers/mmu.h"

static bool* im_memory = NULL;

void im_init() {  //                                       mouse x, y
    SIZE_T total_size = sizeof(bool) * (IM_KEY_COUNT * 2) + 8;

    im_memory = g_malloc(total_size);

    g_memset(im_memory, 0, total_size);
}
void im_begin() {
    g_memcpy(IM_LAST_KEYS_PTR, IM_KEYS_PTR, sizeof(bool) * IM_KEY_COUNT);
}

bool im_key_get(const enum KEYCODES keycode) { return (IM_CURRENT);}
bool im_key_get_down(const enum KEYCODES keycode) {return (IM_CURRENT) && !(IM_PREVIOUS);}
bool im_key_get_up(const enum KEYCODES keycode) {return !(IM_CURRENT) && (IM_PREVIOUS);}

unsigned int im_mouse_pos_x() { return *(unsigned int*)(IM_MOUSE_POS_PTR);}
unsigned int im_mouse_pos_y() { return *(unsigned int*)(IM_MOUSE_POS_PTR + sizeof(unsigned int)); }

#ifdef _WIN32
LRESULT CALLBACK WindowProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    unsigned char wKey;

    switch (uMsg) {
        case WM_MOUSEMOVE:
            *(unsigned int*)(IM_MOUSE_POS_PTR) = (unsigned int)LOWORD(lParam);
            *(unsigned int*)(IM_MOUSE_POS_PTR + sizeof(unsigned int)) = (unsigned int)HIWORD(lParam);
            break;
        case WM_LBUTTONDOWN: {
            unsigned int x = LOWORD(lParam);
            unsigned int y = HIWORD(lParam);

            break;
        }
        case WM_KEYDOWN: {
            if (im_memory == NULL) break;
            wKey = (unsigned char) wParam;
            if (wKey < 256) *(IM_KEYS_PTR + wKey) = true;
            break;
        }
        case WM_KEYUP: {
            if (im_memory == NULL) break;
            wKey = (unsigned char) wParam;
            if (wKey < 256) *(IM_KEYS_PTR + wKey) = false;
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            BITMAPINFO bmi = {};
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = W_W;
            bmi.bmiHeader.biHeight = -W_H;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_BITFIELDS;

            ((DWORD*)bmi.bmiColors)[0] = 0xFF000000;
            ((DWORD*)bmi.bmiColors)[1] = 0x00FF0000;
            ((DWORD*)bmi.bmiColors)[2] = 0x0000FF00;

            StretchDIBits(
                hdc,
                0, 0, W_W, W_H,
                0, 0, W_W, W_H,
                screen_buffer, &bmi,
                DIB_RGB_COLORS, SRCCOPY
            );

            EndPaint(hWnd, &ps);
            return 0;
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#endif
