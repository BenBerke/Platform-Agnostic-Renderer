//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_APP_H
#define MINIFB_APP_H

#ifdef  __WIN32

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void generic_main();

#endif

#endif //MINIFB_APP_H