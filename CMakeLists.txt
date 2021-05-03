cmake_minimum_required(VERSION 3.17)
project(Algorithm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS "-O2")

add_executable(Algorithm main.cpp AVL.c AVLTest.cpp sortTest.cpp)