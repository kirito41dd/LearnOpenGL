#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <breakout/texture.h>
#include <breakout/sprite_renderer.h>
#include <learnopengl/shader_s.h>

// PostProcessor 负责所有的后期处理
// 在渲染之前使用 BeginRender() 渲染后使用 EndRender()
class PostProcessor
{
public:
    Shader PostProcessingShader;
    Texture2D Texture;
    GLuint Width, Height;
    // 选项
    GLboolean Confuse, Chaos, Shake; // 混乱(颠倒方向) 混沌（镜头移动）  晃动

    PostProcessor(Shader shader, GLuint width, GLuint height);
    // 在渲染游戏之前准备后期处理的帧缓冲区操作
    void BeginRender();
    // 应该在渲染游戏后调用，它把所有渲染的数据存储到一个纹理对象中
    void EndRender();
    // 渲染后期处理纹理四边形
    void Render(GLfloat time);

private:
    GLuint MSFBO, FBO; // MSFBO = Multisampled FBO 多重采样帧缓冲， FBO是常规帧缓冲，用于将多重采样的颜色缓冲渲染到纹理上
    GLuint RBO; // 缓冲对象附件 RBO is used for multisampled color buffer 用于多重采样颜色缓冲
    GLuint VAO;
    // 为后期处理纹理 初始化四边形
     void initRenderData();
}; // class PostProcessor

#endif // POST_PROCESSOR_H
