# 你好，窗口
add_executable(HelloWindow HelloWindow.cpp)
target_link_libraries(HelloWindow ${GLFW3_LIBRARY} glad)

# 你好，三角形
add_executable(HelloTriangle HelloTriangle.cpp)
target_link_libraries(HelloTriangle ${GLFW3_LIBRARY} glad)

add_executable(HelloTriangle_2 HelloTriangle_2.cpp)
target_link_libraries(HelloTriangle_2 ${GLFW3_LIBRARY} glad)

# 着色器
add_executable(Shaders_uniform Shaders_uniform.cpp)
target_link_libraries(Shaders_uniform ${GLFW3_LIBRARY} glad)

add_executable(Shaders_2 Shaders_2.cpp)
target_link_libraries(Shaders_2 ${GLFW3_LIBRARY} glad)

# 纹理
add_executable(Textures Textures.cpp)
target_link_libraries(Textures ${GLFW3_LIBRARY} glad stb_image)

add_executable(Textures_mix Textures_mix.cpp)
target_link_libraries(Textures_mix ${GLFW3_LIBRARY} glad stb_image)

# 变换
add_executable(Transformation Transformation.cpp)
target_link_libraries(Transformation ${GLFW3_LIBRARY} glad stb_image)

# 3d
add_executable(Start3d Start3d.cpp)
target_link_libraries(Start3d ${GLFW3_LIBRARY} glad stb_image)

add_executable(More3d More3d.cpp)
target_link_libraries(More3d ${GLFW3_LIBRARY} glad stb_image)

add_executable(More3d_2 More3d_2.cpp)
target_link_libraries(More3d_2 ${GLFW3_LIBRARY} glad stb_image)

# 摄像机 Camera
add_executable(Camera Camera.cpp)
target_link_libraries(Camera ${GLFW3_LIBRARY} glad stb_image)

add_executable(Camera_move Camera_move.cpp)
target_link_libraries(Camera_move ${GLFW3_LIBRARY} glad stb_image)

# 渲染文本
add_executable(RenderText RenderText.cpp)
target_link_libraries(RenderText ${GLFW3_LIBRARY} glad ${FREETYPE2_LIBRARY})