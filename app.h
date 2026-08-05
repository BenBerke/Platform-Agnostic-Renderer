//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_APP_H
#define MINIFB_APP_H

#define UNICODE
#define _UNICODE

#define BOOL char
#define TRUE 1
#define FALSE 0

#include <windows.h>

typedef struct APP_INFO {
    int x;
} APP_INFO;

// === Forward Declarations ===
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif //MINIFB_APP_H