# 颜色
add_executable(Colors Colors.cpp)
target_link_libraries(Colors glfw3 glad)

# 基础光照
add_executable(Basic_lighting Basic_lighting.cpp)
target_link_libraries(Basic_lighting glfw3 glad)

# 材质
add_executable(Materials Materials.cpp)
target_link_libraries(Materials glfw3 glad)

# 光照贴图
add_executable(Lighting_maps Lighting_maps.cpp ../../include/stb_image_wrap.cpp)
target_link_libraries(Lighting_maps glfw3 glad)