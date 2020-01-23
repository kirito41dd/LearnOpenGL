// 测试 learnopengl/shader_s.h
#include <learnopengl/shader_s.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("usage: %s vertexShaderPath fragmentShaderPath\n",argv[0]);
        exit(0);
    }

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

    // 着色器
   Shader ourShader(argv[1], argv[2]);
    

    // 设置顶点数据 (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // 位置             // 颜色
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 右下
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // 顶部
    };
    // 1.绑定VAO 2.bind and set vertex buffer 3. 配置 vertex attributes
    unsigned int VAO, VBO; 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); // 绑定VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    
    // 配置顶点属性
    // 顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0); // 注意步长是 6
    // 第一个参数指定我们要配置的顶点属性 layout(location = 0)定义了position顶点属性的位置值(Location) 所以传入0
    // 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
    // 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
    // 参数四定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
    // 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)
    // 最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。
    glEnableVertexAttribArray(0); // 启用顶点属性；顶点属性默认是禁用的。
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 调用glVertexAttribPointer将VBO注册为顶点属性的绑定顶点缓冲区对象，因此之后我们可以安全地解除绑定
    glBindVertexArray(0); // 可以取消绑定VAO，这样其他VAO调用就不会意外修改此VAO 修改其他VAO都需要调用glBindVertexArray，所以不是特别需要不用解绑（VAO 和 VBO）

    // 取消注释 将绘制线框多边形
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_FILL 表示实心

    // 渲染循环 render loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window); // 处理输入

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

        // 画三角
        //glUseProgram(shaderProgram);
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 参数: 图元类型 顶点数组起始索引 绘制多少个顶点


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