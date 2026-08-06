//
// Created by berke on 8/5/2026.
//

#ifndef MINIFB_MMU_H
#define MINIFB_MMU_H

#include "../app.h"

// Memory management Unit

static inline void* g_malloc_raw(unsigned long long size) {
#ifdef _WIN32
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    return NULL;
#endif
}

static inline void g_copy_mem(void *dst, void *src, unsigned long long size) {
#ifdef _WIN32
    CopyMemory(dst, src, size);
#endif
}

static inline void g_memset(void *dst, int const val, const unsigned long long size) {
#ifdef _WIN32
    FillMemory(dst, val, size);
#endif
}

#ifdef __cplusplus
#define g_malloc(type, size) static_cast<type*>(g_malloc_raw(size))
#else
#define g_malloc(size) (g_malloc_raw(size))
#endif

#endif //MINIFB_MMU_H