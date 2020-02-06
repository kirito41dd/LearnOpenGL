#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <breakout/texture.h>
#include <breakout/sprite_renderer.h>
#include <breakout/game_object.h>

class BallObject : public GameObject
{
public:
    GLfloat Radius; // 半径
    GLboolean Stuck; // 粘住

    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

    // 移动
    glm::vec2 Move(GLfloat dt, GLuint window_width);
    // 重置位置和速度
    void Reset(glm::vec2 position, glm::vec2 velocity);

}; // clas BallObject

#endif // BALL_OBJECT_H
