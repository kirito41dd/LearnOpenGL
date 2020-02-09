#include <breakout/particle_generator.h>
#include <stdlib.h>

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
    : shader(shader),
      texture(texture),
      amount(amount)
{
    this->init();
}

ParticleGenerator::~ParticleGenerator()
{
    glDeleteVertexArrays(1, &this->VAO);
}

void ParticleGenerator::Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset)
{
    // 添加新的粒子
    for (GLuint i = 0; i < newParticles; ++i)
    {
        this->respawnParticle(this->particles[this->newidx], object, offset);
        this->newidx = ++this->newidx % this->amount;
    }
    // 更新所有粒子状态
    for (GLuint i=0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f) // 粒子向屏幕上方移动 颜色变浅
        {
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5;
        }
    }
}

void ParticleGenerator::Draw()
{
    // 发光的效果
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for (Particle &particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->shader.setVec2("offset", particle.Position);
            this->shader.setVec4("color", particle.Color);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // 把混合恢复
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // 设置顶点属性
    GLuint VBO;
    GLfloat particle_quad[] = { // 图形坐标  纹理坐标
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    // TODO: VBO 释放了吗

    // 创建粒子实例
    for (GLuint i = 0; i < this->amount; ++i)
    {
        this->particles.push_back(Particle());
    }
}


void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
    GLfloat random = ((rand()%100) - 50) / 10.0f;
    GLfloat rColor = 0.5 + ((rand()%100) / 100.0f);
    particle.Position = object.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}
