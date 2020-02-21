# 测试 shader类
add_executable(test_shader_s_h test_shader_s.h.cpp)
target_link_libraries(test_shader_s_h ${GLFW3_LIBRARY} glad)

# 测试 camera类
add_executable(test_camera_h test_camera_h.cpp)
target_link_libraries(test_camera_h ${GLFW3_LIBRARY} glad stb_image)
