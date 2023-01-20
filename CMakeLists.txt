cmake_minimum_required(VERSION 3.22.1)
project(code)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O3)

add_executable(main_v1 v1/main_v1.cpp v1/quicksort_v1.h v1/heapsort_v1.h v1/insertion_sort_v1.h definitions.h utils.h)
add_executable(main_v2 v2/main_v2.cpp v2/quicksort_v2.h definitions.h utils.h)
add_executable(main_v3 v3/main_v3.cpp v3/quicksort_v3.h definitions.h utils.h)