cmake_minimum_required(VERSION 3.22)

project(interpreter CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(interpreter src/main.cpp src/matrix.cpp src/matrix_interpreter.cpp)
