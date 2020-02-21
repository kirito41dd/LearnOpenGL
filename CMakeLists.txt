cmake_minimum_required(VERSION 2.6)

project(learnopengl)

# 依赖环境
# ------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
add_library(glad include/glad.c)
add_library(stb_image include/stb_image_wrap.cpp)
find_package(GLFW3 REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(FREETYPE2 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${FREETYPE2_INCLUDE_DIR})
# ------------------------------------------------------------------------------

include_directories(./include)

# 子目录
add_subdirectory(src/start)
add_subdirectory(src/lighting)
add_subdirectory(src/model)
add_subdirectory(src/advance)
add_subdirectory(src/breakout)

add_subdirectory(test)
