cmake_minimum_required(VERSION 4.4)
project(MiniFB C)

set(CMAKE_C_STANDARD 99)

add_executable(MiniFB main.c
        fenster.h
        src/renderer.c
        headers/renderer.h)
