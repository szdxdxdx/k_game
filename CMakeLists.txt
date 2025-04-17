
cmake_minimum_required(VERSION 3.20)

project(k_game C)

set(CMAKE_C_STANDARD 90)

file(GLOB INCLUDE_SUBDIRS "${CMAKE_SOURCE_DIR}/include/*")
foreach(INCLUDE_SUBDIR ${INCLUDE_SUBDIRS})
    if(IS_DIRECTORY ${INCLUDE_SUBDIR})
        include_directories(${INCLUDE_SUBDIR})
    endif()
endforeach()

include_directories(
    "${CMAKE_SOURCE_DIR}/libs/SDL2-devel-2.30.11-mingw/SDL2-2.30.11/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_image-devel-2.8.4-mingw/SDL2_image-2.8.4/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include/SDL2"
    "${CMAKE_SOURCE_DIR}/libs/webui-windows-gcc-x64/include")

file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/test/*.c"
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
