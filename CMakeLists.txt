cmake_minimum_required(VERSION 3.13)
project(PPM_Converter C)

file(GLOB SOURCES
        "src/*.h"
        "src/*.c"
        )

set(CMAKE_C_STANDARD 99)
include_directories(src)

add_executable(PPM_Converter ${SOURCES})
add_executable(PPM_Tests ${SOURCES} tests/test_matrix.c)