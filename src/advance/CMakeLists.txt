# 深度测试
add_executable(Depth_testing Depth_testing.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Depth_testing glfw3 glad)