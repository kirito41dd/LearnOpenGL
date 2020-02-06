#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

// Texture2D类负责存储和配置纹理
class Texture2D
{
public:
    GLuint  ID; // 保存纹理对象的ID
    GLuint  Width, Height; // 加载图像的宽度和高度(以像素为单位)
    // 纹理格式
    GLuint  Internal_Format; // 纹理对象格式
    GLuint  Image_Format; // 图像格式
    // 纹理设置
    GLuint Wrap_S; // S轴环绕模式
    GLuint Wrap_T; // T轴环绕模式
    GLuint Filter_Min; // 过滤模式 缩小
    GLuint Filter_Max; // 过滤模式 放大

    Texture2D();
    ~Texture2D() = default;

    // 从图像数据生成纹理
    void Generate(GLuint width, GLuint height, unsigned char* data);
    // 绑定纹理为当前 GL_TEXTURE_2D texture object
    void Bind() const;

}; // class Texture2D


#endif // TEXTURE_H
