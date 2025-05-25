
cmake_minimum_required(VERSION 3.20)

project(my_game C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

add_subdirectory("k_game")

add_subdirectory("example/demo_test")
