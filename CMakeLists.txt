cmake_minimum_required(VERSION 3.22.1)
project(code)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O3)

add_executable(main main.cpp quicksort.h counting_sort.h heapsort.h insertion_sort.h definitions.h utils.h)
set_target_properties(main PROPERTIES LINK_FLAGS -Wl,--stack,16777216)