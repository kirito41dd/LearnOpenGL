// 渲染文本
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// freetype
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include <string>

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void RenderText(Shader &shader, string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

// 保存与使用FreeType加载的字符相关的所有状态信息
struct Character
{
    GLuint      TextureID;
    glm::ivec2  Size;
    glm::ivec2  Bearing; // 从基线到字形左侧/顶部的偏移
    GLuint      Advance; // 前进到下一个字形的水平偏移量
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;


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

    // glad 加载所以OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    // OpenGL要求所有的纹理都是4字节对齐的，即纹理的大小永远是4字节的倍数。通常这并不会出现什么问题，因为大部分纹理的宽度都为4的倍数并/或每像素使用4个字节，
    // 但是现在我们每个像素只用了一个字节，它可以是任意的宽度。通过将纹理解压对齐参数设为1，这样才能确保不会有对齐问题（它可能会造成段错误）。

    // 编译着色器
    string vs,fs;
    vs = root + "shader/start/rendertext.vs";
    fs = root + "shader/start/rendertext.fs";
    Shader ourShader(vs.c_str(), fs.c_str());
    
    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -0.1f, 100.0f);
    ourShader.use();
    ourShader.setMat4("projection", projection);

    // 初始化 freetype
    // 所有函数都返回一个不同于0的值表示错误
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
    }
    FT_Face face;
    string fontpath = root + "fonts/arial.ttf";
    if (FT_New_Face(ft, fontpath.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    // 设置字形大小
    FT_Set_Pixel_Sizes(face, 0, 48); // 宽度为0表示通过高度计算

    // 加载 128 个ASCII字符
    for (GLubyte c = 0; c < 128; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // 生成纹理
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));

    } // end for
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // 释放freetype
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // 设置顶点
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // 绘制字符时经常更新VBO的内存，我们用动态方式
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    
    // 渲染循环 render loop
    while(!glfwWindowShouldClose(window))
    {

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

        RenderText(ourShader, "Render text", 25.0f, 400.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText(ourShader, "Hello world!", 25.0f, 200.0f, 2.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText(ourShader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText(ourShader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        glfwSwapBuffers(window); // 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲）
        glfwPollEvents(); // 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
    }

    // 释放之前分配的资源
    glfwTerminate();
    return 0;
}

void RenderText(Shader &shader, string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    shader.use();
    shader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // 遍历字符
    for (int i=0; i < text.length(); ++i)
    {
        char c = text[i];
        Character ch = Characters[c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // 对每个字符更新vbo
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };

        // 纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新下一个字形的x
        x += (ch.Advance >> 6) * scale; //  单位是1/64像素(2^6 = 64)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}