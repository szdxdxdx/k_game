
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "./src/*.c")

add_library(k_game_obj OBJECT ${SRC_FILES})

file(GLOB INCLUDE_SUBDIRS "./include/*")
foreach(INCLUDE_SUBDIR ${INCLUDE_SUBDIRS})
    if(IS_DIRECTORY ${INCLUDE_SUBDIR})
        target_include_directories(k_game_obj PUBLIC ${INCLUDE_SUBDIR})
    endif()
endforeach()

target_include_directories(k_game_obj PUBLIC
    "./libs/SDL2-devel-2.30.11-mingw/SDL2-2.30.11/x86_64-w64-mingw32/include/SDL2"
    "./libs/SDL2_image-devel-2.8.4-mingw/SDL2_image-2.8.4/x86_64-w64-mingw32/include/SDL2"
    "./libs/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include/SDL2"
    "./libs/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include/SDL2"
    "./libs/webui-windows-gcc-x64/include")

# ------------------------------------------------------------------------------------------------------

add_library(k_game STATIC $<TARGET_OBJECTS:k_game_obj>)

foreach(INCLUDE_SUBDIR ${INCLUDE_SUBDIRS})
    if(IS_DIRECTORY ${INCLUDE_SUBDIR})
        target_include_directories(k_game PUBLIC ${INCLUDE_SUBDIR})
    endif()
endforeach()

target_link_directories(k_game PUBLIC
    "./libs/SDL2-devel-2.30.11-mingw/SDL2-2.30.11/x86_64-w64-mingw32/lib/"
    "./libs/SDL2_image-devel-2.8.4-mingw/SDL2_image-2.8.4/x86_64-w64-mingw32/lib/"
    "./libs/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/lib/"
    "./libs/SDL2_ttf-devel-2.24.0-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib/"
    "./libs/webui-windows-gcc-x64/")

target_link_libraries(k_game PUBLIC
    SDL2_image SDL2_mixer SDL2_ttf SDL2
    webui-2-static
    ws2_32)

# ------------------------------------------------------------------------------------------------------

# 为项目添加宏 `K__FILE_PATH`，展开结果为所在文件相对于项目根目录的相对路径
get_target_property(TARGET_SRCS k_game SOURCES)
foreach(TARGET_SRC ${TARGET_SRCS})
    if(IS_ABSOLUTE ${TARGET_SRC})
        file(RELATIVE_PATH TARGET_SRC_RELPATH ${PROJECT_SOURCE_DIR} ${TARGET_SRC})
    else()
        set(TARGET_SRC_RELPATH ${TARGET_SRC})
    endif()
    set_property(SOURCE ${TARGET_SRC} APPEND PROPERTY COMPILE_DEFINITIONS "K__FILE_PATH=\"${TARGET_SRC_RELPATH}\"")
endforeach()
