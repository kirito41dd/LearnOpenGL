// 你好，三角形 2
// 假设我们不再绘制一个三角形而是绘制一个矩形。我们可以绘制两个三角形来组成一个矩形（OpenGL主要处理三角形）
// 一个矩形只有4个而不是6个顶点，这样就产生50%的额外开销。更好的解决方案是只储存不同的顶点，并设定绘制这些顶点的顺序。
// 索引缓冲对象(EBO)的工作方式正是这样的
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// 用着色器语言GLSL(OpenGL Shading Language)编写顶点着色器
const char *vertexShaderSource =    "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                    "}\n\0";
// 片段着色器
const char *fragmentShaderSource =  "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 橘色
                                    "}\0";


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
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
    // 顶点着色器
    int vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建着色器， 返回ID, 参数是着色器类型
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 把这个着色器源码附加到着色器对象上,参数：着色器 字符串数量 源码字符串 NULL
    glCompileShader(vertexShader); // 编译着色器
    // 检查是否成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // 获取编译状态
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "vertexShader compilation failed \n" << infoLog << endl;
    }

    // 片段着色器
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "fragmentShader compilation failed \n" << infoLog << endl;
    }

    // 链接
    // 要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象
    int shaderProgram = glCreateProgram(); // 创建程序对象，返回引用ID
    glAttachShader(shaderProgram, vertexShader); // 附加
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // 链接
    // 检查
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "shaderProgram linking failed \n" << infoLog << endl;
    }
    // 删除着色器，他们已经不需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    

    // 设置顶点数据 (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    unsigned int indices[] = { // 注意索引从0开始!  逆时针组合
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    // 1.绑定VAO 2.bind and set vertex buffer 3. 配置 vertex attributes
    unsigned int VAO; // 顶点数组对象 ID
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO); // 绑定VAO

    unsigned int VBO; // 顶点缓冲对象 ID
    glGenBuffers(1, &VBO); // 像OpenGL中的其它对象一样，这个缓冲有一个独一无二的ID，所以我们可以使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
    // OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
    // 第一个参数是目标缓冲的类型 第二个参数指定传输数据的大小 第三个参数是我们希望发送的实际数据 四个参数指定了我们希望显卡如何管理给定的数据

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 配置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    // 第一个参数指定我们要配置的顶点属性 layout(location = 0)定义了position顶点属性的位置值(Location) 所以传入0
    // 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
    // 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
    // 参数四定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
    // 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)
    // 最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。
    glEnableVertexAttribArray(0); // 启用顶点属性；顶点属性默认是禁用的。

    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 调用glVertexAttribPointer将VBO注册为顶点属性的绑定顶点缓冲区对象，因此之后我们可以安全地解除绑定
    glBindVertexArray(0); // 可以取消绑定VAO，这样其他VAO调用就不会意外修改此VAO 修改其他VAO都需要调用glBindVertexArray，所以不是特别需要不用解绑（VAO 和 VBO）
    // 在VAO解绑之前，不要解绑EBO，否则EBO不会保存在VAO中

    // 取消注释 将绘制线框多边形
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_FILL 表示实心

    // 渲染循环 render loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window); // 处理输入

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

        glUseProgram(shaderProgram); //函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象
        glBindVertexArray(VAO); // 因为我们只有一个VAO，所以没有必要每次都绑定它，但是我们这样做是为了让事情更有条理
        //glDrawArrays(GL_TRIANGLES, 0, 3); // 参数: 图元类型 顶点数组起始索引 绘制多少个顶点
        // glDrawElements函数从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取索引
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 参数： 图元类型 绘制顶点个数 索引的类型 EBO中的偏移量
        // glBindVertexArray(0); // no need to unbind it every time 

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