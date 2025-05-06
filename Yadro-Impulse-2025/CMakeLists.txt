cmake_minimum_required(VERSION 3.10)

project(yadro-project CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
add_executable(tape-sort src/main.cpp src/tape.cpp src/sort.cpp)

# need command
add_executable(tape-sort-test EXCLUDE_FROM_ALL test/main_test.cpp src/tape.cpp src/sort.cpp)
add_executable(tape-make EXCLUDE_FROM_ALL test/tape_make.cpp)
set_target_properties(tape-sort-test  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/"
)
set_target_properties(tape-make  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/"
)
list(APPEND TEST_TARGET tape-sort-test)
list(APPEND TEST_TARGET tape-make)
add_custom_target(tests DEPENDS ${TEST_TARGET})