
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(k_game STATIC ${SRC_FILES})

file(GLOB INCLUDE_SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/*")
foreach(INCLUDE_SUBDIR ${INCLUDE_SUBDIRS})
    if(IS_DIRECTORY ${INCLUDE_SUBDIR})
        target_include_directories(k_game PUBLIC ${INCLUDE_SUBDIR})
    endif()
endforeach()

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
