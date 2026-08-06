cmake_minimum_required(VERSION 4.4)
project(MiniFB C)

set(CMAKE_C_STANDARD 99)

add_executable(MiniFB main.c
        app.h
        src/app.c
        src/debug.c
        src/input_manager.c
        headers/input_manager.h
        globals.h
        headers/debug.h
        headers/renderer.h
        src/renderer.c
        src/generic_main.c
        headers/mmu.h
        src/mmu.c
        headers/math.h
        src/math.c
        headers/time.h
        src/time.c
)
