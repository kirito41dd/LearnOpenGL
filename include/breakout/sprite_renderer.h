// 渲染精灵
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <breakout/texture.h>
#include <learnopengl/shader_s.h>

class SpriteRenderer
{
public:
    SpriteRenderer(Shader &Shader);
    ~SpriteRenderer();
    //用给定的精灵渲染一个定义好的四边形纹理
    void DrawSprite(Texture2D &texture,
                    glm::vec2 position,
                    glm::vec2 size = glm::vec2(10, 10),
                    GLfloat rotate = 0.0f,                  // 旋转
                    glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    GLuint quadVAO;
    // 初始化和配置四边形的缓冲区和顶点属性
    void initRenderData();
}; // class SpriteRenderer

#endif // SPRITE_RENDERER_H
