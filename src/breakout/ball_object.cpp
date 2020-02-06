#include <breakout/ball_object.h>

BallObject::BallObject()
    : GameObject(),
      Radius(12.5f),
      Stuck(true)
{

}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity),
      Radius(radius),
      Stuck(true)
{

}

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width)
{
    // 如果没有黏在球板上
    if (!this->Stuck)
    {
        // 移动
        this->Position += this->Velocity * dt;
        //检查是否超出窗口界限，如果是，反转速度并恢复到正确的位置
        if (this->Position.x <= 0.0f) // 左边界
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= window_width) // 右边界
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if (this->Position.y <= 0.0f) // 上边界
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
        // 我们不管下边界
    }
    return this->Position;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}
