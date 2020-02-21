# 深度测试
add_executable(Depth_testing Depth_testing.cpp)
target_link_libraries(Depth_testing ${GLFW3_LIBRARY} glad stb_image)
# 模板测试
add_executable(Stencil_testing Stencil_testing.cpp)
target_link_libraries(Stencil_testing ${GLFW3_LIBRARY} glad stb_image)

# 混合 丢弃片段
add_executable(Blending_discard Blending_discard.cpp)
target_link_libraries(Blending_discard ${GLFW3_LIBRARY} glad stb_image)

# 混合 半透明
add_executable(Blending_translucent Blending_translucent.cpp)
target_link_libraries(Blending_translucent ${GLFW3_LIBRARY} glad stb_image)

# 面剔除
add_executable(FaceCulling FaceCulling.cpp)
target_link_libraries(FaceCulling ${GLFW3_LIBRARY} glad stb_image)

# 帧缓冲
add_executable(Framebuffers Framebuffers.cpp)
target_link_libraries(Framebuffers ${GLFW3_LIBRARY} glad stb_image)

# 帧缓冲 后期处理
add_executable(Framebuffers_post_processing Framebuffers_post_processing.cpp)
target_link_libraries(Framebuffers_post_processing ${GLFW3_LIBRARY} glad stb_image)

# 立方体贴图 天空盒
add_executable(Cubemaps_skybox Cubemaps_skybox.cpp)
target_link_libraries(Cubemaps_skybox ${GLFW3_LIBRARY} glad stb_image)

# 立方体贴图 反射
add_executable(Cubemaps_reflection Cubemaps_reflection.cpp )
target_link_libraries(Cubemaps_reflection ${GLFW3_LIBRARY} glad stb_image)

# 立方体贴图 折射
add_executable(Cubemaps_refraction Cubemaps_refraction.cpp)
target_link_libraries(Cubemaps_refraction ${GLFW3_LIBRARY} glad stb_image)