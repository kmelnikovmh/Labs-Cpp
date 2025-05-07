cmake_minimum_required(VERSION 3.22)

project(biocad-first-json-viewer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)  # Recommended by Boost.DLL's manual

find_package(Boost 1.71 REQUIRED filesystem)  # Required by Boost.DLL
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(include)

add_executable(json-viewer src/main.cpp src/json_interpreter.cpp)
target_link_libraries(json-viewer ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})

add_library(view-compact SHARED src/view_compact.cpp src/json_interpreter.cpp)
