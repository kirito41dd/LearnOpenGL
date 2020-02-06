#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <glad/glad.h>
#include <breakout/texture.h>
#include <learnopengl/shader_s.h>

// 静态单例ResourceManager类，可承载多个加载纹理和着色器的功能。
// 每个加载的纹理或着色器可以通过字符串引用句柄。
// 所有功能和资源都是静态的，没有定义公共构造函数。
class ResourceManager
{
public:
    // 资源存储
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    // 从文件加载着色器
    static Shader& LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
    // 获取着色器
    static Shader& GetShader(std::string name);
    // 从文件加载纹理
    static Texture2D& LoadTexture(const GLchar *file, std::string name);
    // 获取纹理
    static Texture2D& GetTexture(std::string name);
    // 正确释放所有加载的资源
    static void Clear();

private:
    ResourceManager(){} // 无法构造
    // 从文件加载并生成着色器
    static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    // 从文件加载纹理
    static Texture2D loadTextureFromFile(const GLchar *file);

}; // class ResourceManager

#endif // RESOURCE_MANAGER_H
