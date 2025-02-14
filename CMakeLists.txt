
cmake_minimum_required(VERSION 3.20)

project(k_game C)

set(CMAKE_C_STANDARD 11)

include_directories(
    "${CMAKE_SOURCE_DIR}/include/k_game"
    "${CMAKE_SOURCE_DIR}/include/k_list"
    "${CMAKE_SOURCE_DIR}/include/k_log"
    "${CMAKE_SOURCE_DIR}/include/k_printf"
    "${CMAKE_SOURCE_DIR}/include/k_utils")

include_directories(
    "${CMAKE_SOURCE_DIR}/libs/SDL2-devel-2.30.11-mingw/SDL2-2.30.11/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_image-devel-2.8.4-mingw/SDL2_image-2.8.4/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/webui-windows-gcc-x64/include")

file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/docs/api/*.h"
    "${CMAKE_SOURCE_DIR}/test/test_2/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.c")

link_directories(
    "${CMAKE_SOURCE_DIR}/libs/SDL2-devel-2.30.11-mingw/SDL2-2.30.11/x86_64-w64-mingw32/lib"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_image-devel-2.8.4-mingw/SDL2_image-2.8.4/x86_64-w64-mingw32/lib"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/lib"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib"
    "${CMAKE_SOURCE_DIR}/libs/webui-windows-gcc-x64")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(TARGET ${PROJECT_NAME})

add_executable(${TARGET} ${SRC_FILES})

target_link_libraries(${TARGET}
    SDL2  SDL2_image  SDL2_mixer  SDL2_ttf
    webui-2-static  ws2_32)
