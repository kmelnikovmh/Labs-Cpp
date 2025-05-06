cmake_minimum_required(VERSION 3.10)

project(yadro-project CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
add_executable(tape-sort src/main.cpp src/tape.cpp src/sort.cpp)
