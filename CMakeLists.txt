cmake_minimum_required(VERSION 3.13)
project(PPM_Converter C)

file(GLOB SOURCES
        "src/*.h"
        "src/*.c"
        )

set(CMAKE_C_STANDARD 99)
include_directories(src)
find_package (Threads)
add_executable(PPM_Converter ${SOURCES} src/shared/log.c src/shared/log.h)
add_executable(PPM_Tests ${SOURCES} tests/test_matrix.c src/shared/log.c src/shared/log.h)
target_link_libraries(PPM_Converter m ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(PPM_Tests m ${CMAKE_THREAD_LIBS_INIT})
