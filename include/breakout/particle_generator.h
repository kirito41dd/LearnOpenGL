// 粒子生成
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <vector>
#include <glad/glad.h>
#include <learnopengl/shader_s.h>
#include <breakout/texture.h>
#include <breakout/game_object.h>

// 粒子
struct Particle
{
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float   Life;
    Particle()
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f)
    {}
};

class ParticleGenerator
{
public:
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
    ~ParticleGenerator();
    // 在 object 附近产生粒子
    void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();
private:
    std::vector<Particle> particles;
    GLuint amount;
    Shader shader;
    Texture2D texture;
    GLuint VAO;
    GLuint newidx; // 新生成的粒子下标
    void init();
    // 重生
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
}; // class ParticleGenerator



#endif // PARTICLE_GENERATOR_H
