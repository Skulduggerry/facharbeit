cmake_minimum_required(VERSION 3.22.1)
project(code)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O3)
set(CMAKE_EXE_LINKER_FLAGS "-static -Wl,--stack,16777216")

add_executable(main main.cpp intro_sort.h quicksort.h heapsort.h insertion_sort.h definitions.h utils.h)