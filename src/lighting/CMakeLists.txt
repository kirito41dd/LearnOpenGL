# 颜色
add_executable(Colors Colors.cpp)
target_link_libraries(Colors ${GLFW3_LIBRARY} glad)

# 基础光照
add_executable(Basic_lighting Basic_lighting.cpp)
target_link_libraries(Basic_lighting ${GLFW3_LIBRARY} glad)

# 材质
add_executable(Materials Materials.cpp)
target_link_libraries(Materials ${GLFW3_LIBRARY} glad)

# 光照贴图
add_executable(Lighting_maps Lighting_maps.cpp)
target_link_libraries(Lighting_maps ${GLFW3_LIBRARY} glad stb_image)

# 平行光
add_executable(Directional Directional.cpp)
target_link_libraries(Directional ${GLFW3_LIBRARY} glad stb_image)

# 点光源
add_executable(Point Point.cpp)
target_link_libraries(Point ${GLFW3_LIBRARY} glad stb_image)

# 聚光
add_executable(Spot Spot.cpp)
target_link_libraries(Spot ${GLFW3_LIBRARY} glad stb_image)

# 聚光 边缘软化
add_executable(Spot_soft Spot_soft.cpp)
target_link_libraries(Spot_soft ${GLFW3_LIBRARY} glad stb_image)

# 多光源
add_executable(Multiple_lights Multiple_lights.cpp)
target_link_libraries(Multiple_lights ${GLFW3_LIBRARY} glad stb_image)