
# 递归收集 A/src/ 下的所有 .c 文件
file(GLOB_RECURSE DEMO_YX_SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# 创建库或可执行文件
add_executable(demo_yx ${DEMO_YX_SRC_FILES})

# 添加 A/include，仅对 A_module 有效
target_include_directories(demo_yx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(demo_yx PRIVATE
    k_game
    SDL2_image SDL2_mixer SDL2_ttf SDL2
    webui-2-static
    ws2_32)
