cmake_minimum_required(VERSION 4.4)
project(MiniFB C)

set(CMAKE_C_STANDARD 99)

add_executable(MiniFB main.c
        src/math/Vector3.c
        src/math/Vector2.c
        src/math/Matrix3.c
        headers/math/Matrix3.h
        headers/math/Vector2.h
        app.h
        src/app.c
        src/debug.c
        src/inputManager.c
        headers/inputManager.h
        globals.h
        headers/debug.h
)
