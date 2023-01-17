cmake_minimum_required(VERSION 3.24)
project(code)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O3)
set(CMAKE_CXX_FLAGS -Ofast)

add_executable(code main.cpp definitions.h insertion_sort.h quick_sort.h utils.h)
