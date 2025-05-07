cmake_minimum_required(VERSION 3.22)

project(biocad-first-fibonacci CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(fibonacci src/main.cpp src/fib.cpp)