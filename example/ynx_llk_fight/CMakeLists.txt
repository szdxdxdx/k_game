
file(GLOB_RECURSE DEMO_YX_SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_executable(demo_yx ${DEMO_YX_SRC_FILES})

target_include_directories(demo_yx PUBLIC "./")

target_link_libraries(demo_yx PUBLIC k_game)
