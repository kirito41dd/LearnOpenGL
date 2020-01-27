# 测试 shader类
add_executable(test_shader_s_h test_shader_s.h.cpp)
target_link_libraries(test_shader_s_h glfw3 glad)

# 测试 camera类
add_executable(test_camera_h test_camera_h.cpp ../include/stb_image_wrap.cpp)
target_link_libraries(test_camera_h glfw3 glad)

# yh
add_executable(yh yh.cpp ../include/stb_image_wrap.cpp)
target_link_libraries(yh glfw3 glad)