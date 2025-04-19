cmake_minimum_required(VERSION 3.10)

project(bmp-image CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
add_executable(bmp-image src/main.cpp src/image.cpp)

