#include <breakout/resource_manager.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <stb_image.h>

// 实例化静态变量
std::map<std::string, Shader>    ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

Shader& ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::__cxx11::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D& ResourceManager::LoadTexture(const GLchar *file, std::string name)
{
    Textures[name] = loadTextureFromFile(file);
    return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    for (std::map<std::string, Shader>::iterator it = Shaders.begin(); it != Shaders.end(); ++it)
    {
        ::glDeleteProgram(it->second.ID);
    }
    for (std::map<std::string, Texture2D>::iterator it = Textures.begin(); it != Textures.end(); ++it)
    {
        ::glDeleteTextures(1, &(it->second.ID));
    }
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    Shader shader;
    shader.Compile(vShaderFile, fShaderFile, gShaderFile);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file)
{
    Texture2D texture;

    // 加载
    int width, height, nrComponents;
    unsigned char *image = stbi_load(file, &width, &height, &nrComponents, 0);
    /// TODO: 更好的日志打印
    if (image == NULL)
    {
        std::cout << "image load fail:" << file << std::endl;
    }

    if (nrComponents == 4)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
        texture.Wrap_S = GL_CLAMP_TO_EDGE;
        texture.Wrap_T = GL_CLAMP_TO_EDGE;
    }

    texture.Generate(width, height, image);
    stbi_image_free(image);
    return texture;
}
