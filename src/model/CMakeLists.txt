# 模型加载
add_executable(Model_loading Model_loading.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Model_loading glfw3 glad assimp)

# girl
add_executable(Model_girl Model_girl.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Model_girl glfw3 glad assimp)