#ifndef POWER_UP_H
// header only

#define POWER_UP_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include <breakout/game_object.h>

const glm::vec2 PU_SIZE(60, 20);
const glm::vec2 PU_VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
    std::string Type;
    GLfloat     Duration;
    GLboolean   Activated;

    PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
        : GameObject(position, PU_SIZE, texture, color, PU_VELOCITY),
          Type(type),
          Duration(duration),
          Activated(false)
    {}
}; // class PowerUp

#endif // POWER_UP_H
