//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_MMU_H
#define MINIFB_MMU_H

#include "../app.h"
#include "../globals.h"
#include "../headers/typedefs.h"

// Memory management Unit
static inline void* g_malloc_raw(const u64 size) {
#ifdef _WIN32
    if (size == 0) return NULL;
    return HeapAlloc(GetProcessHeap(), 0, (usize)size);
#else
    return NULL;
#endif
}

static inline void g_free_raw(void* ptr) {
    if (ptr == NULL) return;
#ifdef _WIN32
    HeapFree(GetProcessHeap(), 0, ptr);
#else
    return NULL;
#endif
}

static inline void* g_realloc_raw(void* ptr, const u64 size) {
#ifdef _WIN32
    HANDLE heap = GetProcessHeap();

    if (ptr == NULL) return g_malloc_raw(size);
    if (size == 0) {
        HeapFree(heap, 0, ptr);
        return NULL;
    }

    return HeapReAlloc(heap, 0, ptr, size);
#else
    return NULL;
#endif
}

static inline void g_memcpy(void *dst, const void *src, const u64 size) {
#ifdef _WIN32
    CopyMemory(dst, src, size);
#else
    return NULL;
#endif
}

static inline int  g_memcmp_raw(void* a, const void* b, const u64 size) {
    const u8* left = (const u8*)a;
    const u8* right = (const u8*)b;

    for (u64 i = 0; i < size; i++) {
        if (left[i] < right[i]) return -1;
        if (left[i] > right[i]) return 1;
    }
    return 0;
}

static inline void g_memset_raw(void *dst, const char val, const u64 size) {
#ifdef _WIN32
    FillMemory(dst, size, val);
#else
    return NULL;
#endif
}

#define g_realloc(ptr, size) (g_realloc_raw(ptr, size))
#define g_free(ptr) (g_free_raw(ptr))
#define g_malloc(size) (g_malloc_raw(size))
#define g_memcmp(a, b, size) (g_memcmp_raw(a, b, size))
#define g_memset(a, b, c) (g_memset_raw(a, b, c))

bool g_read_file(const char *file_path, char** out_buffer, u64* out_size);

#endif //MINIFB_MMU_H