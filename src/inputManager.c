//
// Created by berke on 8/5/2026.
//

#include "../headers/inputManager.h"
#include "../app.h"
#include "../globals.h"
#include "../headers/debug.h"

static bool* im_memory = NULL;

void im_init() {
    SIZE_T total_size = sizeof(bool) * (IM_KEY_COUNT * 2);

    im_memory = (bool*)VirtualAlloc(NULL, total_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}
void im_begin() {
    CopyMemory(IM_LAST_KEYS_PTR, IM_KEYS_PTR, sizeof(bool) * IM_KEY_COUNT);
}

bool im_get_key(const enum KEYCODES keycode) {
    return (IM_CURRENT);
}
bool im_get_key_down(const enum KEYCODES keycode) {
    return (IM_CURRENT) && !(IM_PREVIOUS);
}
bool im_get_key_up(const enum KEYCODES keycode) {
    return !(IM_CURRENT) && (IM_PREVIOUS);
}

LRESULT CALLBACK WindowProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    unsigned char wKey;

    switch (uMsg) {
        case WM_MOUSEMOVE:
            if (wParam & MK_LBUTTON) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
            }
            break;
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            break;
        }
        case WM_KEYDOWN: {
            if (im_memory == NULL) break;
            wKey = (unsigned char) wParam;
            if (wKey < 256) *(im_memory + wKey) = true;
            break;
        }
        case WM_KEYUP: {
            if (im_memory == NULL) break;
            wKey = (unsigned char) wParam;
            if (wKey < 256) *(im_memory + wKey) = false;
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
            bmi.bmiHeader.biCompression = BI_RGB;

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