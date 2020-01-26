# 你好，窗口
add_executable(HelloWindow HelloWindow.cpp)
target_link_libraries(HelloWindow glfw3 glad)

# 你好，三角形
add_executable(HelloTriangle HelloTriangle.cpp)
target_link_libraries(HelloTriangle glfw3 glad)

add_executable(HelloTriangle_2 HelloTriangle_2.cpp)
target_link_libraries(HelloTriangle_2 glfw3 glad)

# 着色器
add_executable(Shaders_uniform Shaders_uniform.cpp)
target_link_libraries(Shaders_uniform glfw3 glad)

add_executable(Shaders_2 Shaders_2.cpp)
target_link_libraries(Shaders_2 glfw3 glad)

# 纹理
add_executable(Textures Textures.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Textures glfw3 glad)

add_executable(Textures_mix Textures_mix.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Textures_mix glfw3 glad)

# 变换
add_executable(Transformation Transformation.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Transformation glfw3 glad)

# 3d
add_executable(Start3d Start3d.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Start3d glfw3 glad)

add_executable(More3d More3d.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(More3d glfw3 glad)