//
// Created by berke on 8/5/2026.
//

#include "../app.h"
#include "../headers/debug.h"

void generic_print(const char* text, ...) {
    va_list args;
    va_start(args, text);

    int i = 0;
    char buffer[256];
    int buffer_stack = 0;

    while (text[i] != '\0') {
        if (buffer_stack >= sizeof(buffer)-1) break;
        if (text[i] == '%') {
            i++; // Advance past %
            if (text[i] == 'd') {
                // Read the int as a string
                char num_buffer[33];
                itoa(va_arg(args, int), num_buffer, 10);

                int j = 0;
                while (num_buffer[j] != '\0' && buffer_stack < sizeof(buffer)-1) buffer[buffer_stack++] = num_buffer[j++];
            }
            else if (text[i] == '%') buffer[buffer_stack++] = '%';

            i++;
        }
        else buffer[buffer_stack++] = text[i++];
    }

    va_end(args);

#ifdef _WIN32
    const HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD bytesWritten;

    WriteFile(hStdOut, buffer, (DWORD)buffer_stack, &bytesWritten, NULL);
#endif
}