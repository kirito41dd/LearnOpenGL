#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <breakout/texture.h>
#include <breakout/sprite_renderer.h>

class GameObject
{
public:
    // 对象状态
    glm::vec2   Position, Size, Velocity; // 位置 大小 速度
    glm::vec3   Color;
    GLfloat     Rotation;
    GLboolean   IsSolid; // 是否坚硬
    GLboolean   Destroyed; // 是否已经摧毁
    // 渲染状态
    Texture2D   Sprite;
    //
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    virtual void Draw(SpriteRenderer &renderer);

}; // class GameObject


#endif // GAME_OBJECT_H
