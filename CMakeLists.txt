cmake_minimum_required(VERSION 3.13)
project(PPM_Converter C)

file(GLOB SOURCES
        "src/*.h"
        "src/*.c"
        )

set(CMAKE_C_STANDARD 99)
include_directories(src)
find_package (Threads)
add_executable(PPM_Converter ${SOURCES} src/sobel.c)
add_executable(PPM_Tests ${SOURCES} tests/tests_ppm_sobel.c  src/sobel.c)
target_link_libraries(PPM_Converter m ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(PPM_Tests m ${CMAKE_THREAD_LIBS_INIT})

