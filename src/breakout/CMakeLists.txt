aux_source_directory(. DIR_BASE_SRCS)
add_executable(breakout ${DIR_BASE_SRCS})
target_link_libraries(breakout ${GLFW3_LIBRARY} glad stb_image)
