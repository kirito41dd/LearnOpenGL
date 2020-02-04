// 纹理单元
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#include <stb_image.h> // 图片加载器 三方
#include <iostream>
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "usage: " << argv[0] << " path/to/project/" << endl;
        exit(0);
    }
    string root = argv[1];

    glfwInit(); // 初始化GLFW库
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 配置主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 配置次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS X 使用
#endif

    // 创建一个窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // 设置为当前上下文
     // 设置一个回调，当用户改变窗口的大小的时候，视口也应该被调整。
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad 加载所以OpenGL函数指针
    // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 编译着色器
    string vs,fs;
    vs = root + "shader/start/texture_mix.vs";
    fs = root + "shader/start/texture_mix.fs";
    Shader ourShader(vs.c_str(), fs.c_str());

    

    // 设置顶点数据 (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // 顶点             // 颜色             //  纹理坐标 s t
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 右上角
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 右下角
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // 左上角
    };
    unsigned int indices[] = { // 注意索引从0开始!  逆时针组合
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    // 
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // 绑定VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
    // OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
    // 第一个参数是目标缓冲的类型 第二个参数指定传输数据的大小 第三个参数是我们希望发送的实际数据 四个参数指定了我们希望显卡如何管理给定的数据


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    // 第一个参数指定我们要配置的顶点属性 layout(location = 0)定义了position顶点属性的位置值(Location) 所以传入0
    // 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
    // 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
    // 参数四定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
    // 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)
    // 最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。
    glEnableVertexAttribArray(0); // 启用顶点属性；顶点属性默认是禁用的。
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理坐标属性
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // 加载和创建纹理
    // 因为OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。很幸运，stb_image.h能够在图像加载时帮助我们翻转y轴，只需要在加载任何图像前加入以下语句即可：
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture1, texture2;
    // texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // 所有即将到来的GL纹理2D操作现在对这个纹理对象有效果
    // 设置纹理环绕参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 缩小时
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 放大时
    // 加载图像，创建纹理并生成 mipmap(贴图)
    int width, height, nrChannels;
    string imgfile; imgfile = root + "img/container.jpg";
    unsigned char *data = stbi_load(imgfile.c_str(), &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        std::cerr << "Failed to load texture" << std::endl;
    } 
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // 第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
        // 第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
        // 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
        // 第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
        // 下个参数应该总是被设为0（历史遗留的问题）
        // 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
        // 最后一个参数是真正的图像数据。
        glGenerateMipmap(GL_TEXTURE_2D); // 为当前绑定的纹理自动生成所有需要的多级渐远纹理
    } 
    stbi_image_free(data); // 释放图像
    // texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); // 所有即将到来的GL纹理2D操作现在对这个纹理对象有效果
    // 设置纹理环绕参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 缩小时
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 放大时
    // 加载图像，创建纹理并生成 mipmap(贴图)
    imgfile = root + "img/awesomeface.png";
    data = stbi_load(imgfile.c_str(), &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        std::cerr << "Failed to load texture" << std::endl;
    } 
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // 为当前绑定的纹理自动生成所有需要的多级渐远纹理
    } 
    stbi_image_free(data); // 释放图像
    
    // 告诉opengl每个采样器属于哪个纹理单元(只需要做一次)
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // uniform sampler2D texture1 属于纹理单元 0
    ourShader.setInt("texture2", 1); // uniform sampler2D texture1 属于纹理单元 1

    // 渲染循环 render loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window); // 处理输入

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

        glActiveTexture(GL_TEXTURE0); // 激活纹理单元 0
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2); // 将纹理绑定到相应的纹理单元上

        glBindVertexArray(VAO); 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 参数： 图元类型 绘制顶点个数 索引的类型 EBO中的偏移量

        glfwSwapBuffers(window); // 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲）
        glfwPollEvents(); // 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
    }

    // 释放之前分配的资源
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 设置窗口的维度
    // glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // 如果按下 esc ， 窗口在下次循环将会退出
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}