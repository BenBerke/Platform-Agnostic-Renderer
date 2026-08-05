//
// Created by berke on 8/5/2026.
//

#include "../headers/inputManager.h"
#include "../app.h"
#include "../globals.h"
#include "../headers/debug.h"

struct InputManager {
    BOOL keys[256];
};

static struct InputManager im = {0};

BOOL im_get_key(const enum KEYCODES keycode) {
    return im.keys[keycode];
}

LRESULT CALLBACK WindowProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
    char wKey;
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
            wKey = (unsigned char)wParam;
            if (wKey >= 0 && wKey < 256) im.keys[wKey] = TRUE;
            break;
            case WM_KEYUP:
            wKey = (unsigned char)wParam;
            if (wKey >= 0 && wKey < 256) im.keys[wKey] = FALSE;
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

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
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}