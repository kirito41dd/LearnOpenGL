# 模型加载
add_executable(Model_loading Model_loading.cpp)
target_link_libraries(Model_loading ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} glad stb_image)

# girl
add_executable(Model_girl Model_girl.cpp)
target_link_libraries(Model_girl ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} glad stb_image)