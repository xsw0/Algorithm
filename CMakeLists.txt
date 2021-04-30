cmake_minimum_required(VERSION 3.17)
project(Algorithm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_executable(Algorithm main.cpp AVLTest.c AVL.c sortTest.cpp sortTest.h)