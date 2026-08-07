//
// Created by berke on 8/6/2026.
//

#include "../headers/mmu.h"

bool g_read_file(const char *file_path, char** out_buffer, u64* out_size) {
#ifdef _WIN32
    HANDLE hFile = CreateFileA(
        file_path,
        GENERIC_READ,
        FILE_SHARE_READ, NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) return false;

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(hFile, &file_size)) {
        CloseHandle(hFile);
        return false;
    }

    char* buffer = (char*)g_malloc(file_size.QuadPart + 1);
    if (!buffer) {
        CloseHandle(hFile);
        return false;
    }

    u64 total_read = 0;
    while (total_read < (u64)file_size.QuadPart) {
        u64 remaining = (u64)file_size.QuadPart - (u64)total_read;

        DWORD to_read = (remaining > 0xFFFFFFFF) ? 0xFFFFFFFF : (DWORD)remaining;
        DWORD bytes_read = 0;

        if (!ReadFile(hFile, buffer + total_read, to_read, &bytes_read, NULL) || bytes_read == 0) break;
        total_read += bytes_read;
    }

    CloseHandle(hFile);

    buffer[total_read] = '\0';

    *out_buffer = buffer;
    if (out_size) *out_size = total_read;
    return true;
#else
    return false;
#endif
}
