// 编写、编译、管理着色器是件麻烦事。
// 编写一个类来做这些事情

#ifndef SHADER_S_H
#define SHADER_S_H

#include <glad/glad.h> // 包含glad来获取所有必须的OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // 程序ID
    unsigned int ID;
    
    // 构造器读取并构建着色器
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
    {
        // 读取源码
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // 确保 ifstream 可以抛出异常
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // 将文件的缓冲区内容读入流
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件
            vShaderFile.close();
            fShaderFile.close();
            // 将流转换为 string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cerr << "file not successfully read: " << e.what() << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        // 编译 shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader program
        this->ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // 激活 Shader
    void use()
    {
        glUseProgram(ID);
    }

    // uniform工具函数 查询一个unform的位置值并设置它的值。
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // uniform工具函数 查询一个unform的位置值并设置它的值。
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // uniform工具函数 查询一个unform的位置值并设置它的值。
    void setFloat(const std::string &name, float value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    // 检查错误
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "shader compilation error of type: " << type << "\n" <<infoLog << std::endl;
            }
        }
        else 
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "shader linking error of type: " << type << "\n" <<infoLog << std::endl;
            }
        }
    }
}; // Class Shader

#endif // SHADER_S_H