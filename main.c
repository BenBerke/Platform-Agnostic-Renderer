#include "globals.h"
#include "headers/renderer.h"

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    generic_main();
}

#endif // _WIN32