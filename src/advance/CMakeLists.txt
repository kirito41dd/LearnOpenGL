# 深度测试
add_executable(Depth_testing Depth_testing.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Depth_testing glfw3 glad)
# 模板测试
add_executable(Stencil_testing Stencil_testing.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Stencil_testing glfw3 glad)