
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_executable(demo_test ${SRC_FILES})

target_include_directories(demo_test PUBLIC "./")

target_link_libraries(demo_test PUBLIC k_game)
