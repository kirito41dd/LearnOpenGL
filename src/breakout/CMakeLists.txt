aux_source_directory(. DIR_BASE_SRCS)
add_executable(breakout ${DIR_BASE_SRCS} ../../include/stb_image_wrap.cpp)
target_link_libraries(breakout glfw3 glad irrKlang)
