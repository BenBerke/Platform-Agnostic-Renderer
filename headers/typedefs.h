//
// Created by berke on 8/6/2026.
//

#ifndef MINIFB_TYPEDEFS_H
#define MINIFB_TYPEDEFS_H

#ifdef _WIN32
typedef signed char i8;
typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef signed int i32;
typedef unsigned int u32;
typedef signed long long i64;
typedef unsigned long long u64;
#ifdef _WIN64
typedef unsigned long long usize;
typedef signed long long isize;
#endif
#else
typedef unsigned int usize;
typedef signed int isize;
#endif

#endif //MINIFB_TYPEDEFS_H