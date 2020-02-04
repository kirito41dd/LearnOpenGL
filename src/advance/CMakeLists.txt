# 深度测试
add_executable(Depth_testing Depth_testing.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Depth_testing glfw3 glad)
# 模板测试
add_executable(Stencil_testing Stencil_testing.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Stencil_testing glfw3 glad)

# 混合 丢弃片段
add_executable(Blending_discard Blending_discard.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Blending_discard glfw3 glad)

# 混合 半透明
add_executable(Blending_translucent Blending_translucent.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Blending_translucent glfw3 glad)

# 面剔除
add_executable(FaceCulling FaceCulling.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(FaceCulling glfw3 glad)