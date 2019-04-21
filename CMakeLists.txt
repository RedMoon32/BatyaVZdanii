cmake_minimum_required(VERSION 3.13)
project(PPM_Converter C)

set(CMAKE_C_STANDARD 99)
include_directories(src)
find_package (Threads)
add_executable(PPM_Converter src/ppm.c src/sobel.c src/main.c)
add_executable(PPM_Tests src/ppm.c src/sobel.c tests/tests_ppm_sobel.c)
target_link_libraries(PPM_Converter m ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(PPM_Tests m ${CMAKE_THREAD_LIBS_INIT})

