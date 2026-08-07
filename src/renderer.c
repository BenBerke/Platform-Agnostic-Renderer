//
// Created by berke on 8/5/2026.
//

#include "../headers/renderer.h"

#include "../globals.h"
#include "../headers/math.h"
#include "../headers/mmu.h"

#ifdef _WIN32
BOOL CALLBACK GetThreadWindowsCallBack(HWND hwnd, LPARAM lParam) {
    *(HWND*)lParam = hwnd;
    return FALSE;
}

int r_init_win(const int w, const int h, const char* title) {
    // Fetch HINSTANCE automatically
    HINSTANCE hInstance = GetModuleHandle(NULL);

    const wchar_t CLASS_NAME[] = L"WindowClass";
    wchar_t wTitle[256];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, wTitle, 256);

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassEx(&wc)) return 0;

    RECT rc = {0, 0, W_W, W_H};

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(
      0, CLASS_NAME, wTitle,
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT,
      rc.right - rc.left, rc.bottom - rc.top,
      NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return 1;
}

bool r_poll_events_win() {
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return 0;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

bool r_update_window_win() {
    HWND hwnd = NULL;

    EnumThreadWindows(GetCurrentThreadId(), GetThreadWindowsCallBack, (LPARAM)&hwnd);
    if (hwnd != NULL) {
        ShowWindow(hwnd, SW_SHOW);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);

        return true;
    }

    return false;
}

#endif

bool r_g_poll_events() {
#ifdef _WIN32
    return r_poll_events_win();
#endif
}

void r_g_init_window(const int w, const int h, const char* title) {
#ifdef _WIN32
r_init_win(w, h, title);
#endif
}
void r_g_update_window() {
#ifdef _WIN32
    r_update_window_win();
#endif
}

// ==============
// Draw Functions
// ==============
void r_draw_line(int x, int y, const int x1, const int y1) {
    const int dx = abs(x - x1);
    const int dy = abs(y - y1);

    const char sx = (x < x1) ? 1 : -1;
    const char sy = (y < y1) ? 1 : -1;

    int err = dx - dy;

    for (;;) {
        r_set_pixel(x, y);

        if (x == x1 && y == y1) break;

        const int e2 = err * 2;

        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void r_draw_horizontal_line(const int x, const int x1, int y) {
    const int start_x = MIN(MAX((x < x1) ? x : x1, 0), W_W);
    const int end_x = MIN(MAX((x < x1) ? x1 : x, 0), W_W);
    const int length = end_x - start_x;

    if (y < 0 || y > W_H) return;

#ifdef _WIN32
    __stosd((unsigned long*)(screen_buffer + cord_to_index(start_x, y)), draw_color, length);
#endif
}

void r_draw_fill_rect(const int x, const int y, const int w, const int h) {
    for (int i = y; i < y + h; i++) r_draw_horizontal_line(x, x+w, i);
}

void r_draw_fill_circle(const int x, const int y, const int half_r) {
    const int left_x = x-half_r;
    const int right_x = x + half_r;
    const int top_y = y-half_r;
    const int bottom_y = y + half_r;

    for (int i = left_x; i <= right_x; i++)
        for (int j = top_y; j <= bottom_y; j++)
            if (distance_sqr(i, j, x, y) < half_r*half_r) r_set_pixel(i, j);
}

void r_draw_texture_scaled_raw(const u32* texture, const u32 w, const u32 h, const int x, const int y, const float scale) {
    const int dw = (int)(w * scale);
    const int dh = (int)(h * scale);

    for (int dy = 0; dy < dh; dy++) {
        const int sy = (int)(dy / scale);
        for (int dx = 0; dx < dw; dx++) {
            const int sx =  (int)(dx / scale);

            const int scx = x + dx;
            const int scy = y + dy;

            r_set_draw_color_int((int)texture[sx + sy * w]);
            r_set_pixel(scx, scy);
        }
    }
}

void r_draw_texture_scaled(const Texture* texture, const int x, const int y, const float scale) {
    r_draw_texture_scaled_raw(texture->data, texture->w, texture->h, x, y, scale);
}