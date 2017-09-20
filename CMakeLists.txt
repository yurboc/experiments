cmake_minimum_required(VERSION 2.8)

project(elevator_controller)

#
# Main project
#
set(SOURCE_EXE main.cpp)
set(SOURCE_LIB elevator/elevator.cpp)

include_directories(elevator)

add_library(elevator STATIC ${SOURCE_LIB})
add_executable(elevator_exec ${SOURCE_EXE})

target_link_libraries(elevator_exec elevator)

#
# Tests
#
enable_testing()

add_executable(test_simple_moving test/test_simple_moving.cpp)
target_link_libraries(test_simple_moving elevator)

add_test( NAME simple_moving COMMAND test_simple_moving )
