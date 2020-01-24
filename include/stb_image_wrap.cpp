#define STB_IMAGE_IMPLEMENTATION // 预处理器会修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件了
#include <stb_image.h>

// 在这个文件包含这两行
// 在其他文件使用 stb_image.h 只需要包含头文件，并且和这个文件一起编译
// 如果在每个使用的地方都包含两行，文件多的时候会出现多重定义