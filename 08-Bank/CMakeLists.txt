cmake_minimum_required(VERSION 3.10)

project(bank CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(Threads)
find_package(Boost 1.71 REQUIRED system)

include_directories(include)

add_executable(bank-server src/bank_server.cpp src/bank.cpp)
target_include_directories(bank-server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(bank-server ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS})

# need command
add_executable(bank-test EXCLUDE_FROM_ALL test/bank_test.cpp src/bank.cpp)
add_executable(server-test EXCLUDE_FROM_ALL test/server_test.cpp src/bank.cpp)
set_target_properties(bank-test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/"
)
set_target_properties(server-test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/"
)
list(APPEND TEST_TARGET bank-test)
list(APPEND TEST_TARGET server-test)
add_custom_target(tests DEPENDS ${TEST_TARGET})



