//
// Created by berke on 8/5/2026.
//

#include "../headers/renderer.h"

#include "../globals.h"
#include "../headers/input_manager.h"
#include "../headers/debug.h"

#ifdef _WIN32
int WINAPI r_init_win(const int w, const int h, const char* title) {
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

    //todo renderer
    FillMemory(screen_buffer, SB_SIZE, ~0);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    im_init();

    MSG msg = {0};
    BOOL running = TRUE;
    while (running) {
        im_begin();

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = FALSE;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        if (im_key_get_up(KC_A)) generic_print("A presseed \n");
    }

    return (int)msg.wParam;
}
#endif

void r_generic_init_window(const int w, const int h, const char* title) {
#ifdef _WIN32
r_init_win(w, h, title);
#endif
}
