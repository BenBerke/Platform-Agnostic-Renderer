#include "app.h"
#include "globals.h"
#include "headers/inputManager.h"
#include "headers/debug.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"WindowClass";

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
      0, CLASS_NAME, L"Hello Screen",
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT,
      rc.right - rc.left, rc.bottom - rc.top,
      NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    //todo renderer
    FillMemory(screen_buffer, SB_SIZE, ~0);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    im_init(); // If this is not included, it crashes

    MSG msg = {0};
    BOOL running = TRUE;
    while (running) {
        im_begin();

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = FALSE;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (im_get_key_up(KC_A)) print("A presseed \n");
    }

    return (int)msg.wParam;
}