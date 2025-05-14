
cmake_minimum_required(VERSION 3.20)

project(my_game C)

set(CMAKE_C_STANDARD 90)

add_subdirectory("k_game")
add_subdirectory("example/ynx_llk_fight")

target_include_directories(k_game PUBLIC
    "${CMAKE_SOURCE_DIR}/libs/SDL2-devel-2.30.11-mingw/SDL2-2.30.11/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_image-devel-2.8.4-mingw/SDL2_image-2.8.4/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/webui-windows-gcc-x64/include")

target_link_directories(demo_yx PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-devel-2.30.11-mingw/SDL2-2.30.11/x86_64-w64-mingw32/lib/"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image-devel-2.8.4-mingw/SDL2_image-2.8.4/x86_64-w64-mingw32/lib/"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/lib/"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib/"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/webui-windows-gcc-x64/")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
