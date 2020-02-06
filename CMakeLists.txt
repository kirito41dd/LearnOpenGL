cmake_minimum_required(VERSION 2.6)

project(learnopengl)

# 添加头文件所在目录和库文件目录
# 只需要修根据环境改下面的路径
# ------------------------------------------------------------------------------
include_directories(C:/Program\ Files\ \(x86\)/GLFW/include D:/workspace/glad/include)
link_directories(C:/Program\ Files\ \(x86\)/GLFW/bin D:/workspace/glad/src)
include_directories(D:/workspace/assimp-4.1.0/assimp-4.1.0/include) # cmake生成后 把build/include/assimp/config.h 拷贝到此目录
link_directories(D:/workspace/assimp-4.1.0/build/code)
# ------------------------------------------------------------------------------

include_directories(./include)

# 子目录
add_subdirectory(src/start)
add_subdirectory(src/lighting)
add_subdirectory(src/model)
add_subdirectory(src/advance)
add_subdirectory(src/breakout)

add_subdirectory(test)
