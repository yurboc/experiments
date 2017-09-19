cmake_minimum_required(VERSION 2.8)

project(elevator_controller)

set(SOURCE_EXE main.cpp)
set(SOURCE_LIB elevator/elevator.cpp)

include_directories(elevator)

add_library(elevator STATIC ${SOURCE_LIB})
add_executable(elevator_exec ${SOURCE_EXE})

target_link_libraries(elevator_exec elevator)
