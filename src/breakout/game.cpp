#include <breakout/game.h>
#include <breakout/resource_manager.h>
#include <breakout/sprite_renderer.h>
#include <breakout/game_object.h>
#include <breakout/ball_object.h>
#include <breakout/particle_generator.h>
#include <breakout/post_processor.h>
//#include <irrKlang.h>

#include <algorithm>
#include <iostream>
#include <time.h>
using namespace std;

// 游戏相关的状态数据
SpriteRenderer  *Renderer;
GameObject      *Player;
BallObject      *Ball;
ParticleGenerator   *Particles; // 粒子
PostProcessor   *Effects;
GLfloat         ShakeTime = 0.0f; // 晃动时间

// 声音
//irrklang::ISoundEngine *SoundEngine;


Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE),
      Keys(),
      Width(width),
      Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
}

void Game::Init()
{
    ::srand(::time(NULL));
    // 加载着色器
    ResourceManager::LoadShader("shader/breakout/sprite.vs", "shader/breakout/sprite.fs", NULL, "sprite");
    ResourceManager::LoadShader("shader/breakout/particle.vs", "shader/breakout/particle.fs", NULL, "particle");
    ResourceManager::LoadShader("shader/breakout/post_processing.vs", "shader/breakout/post_processing.fs", NULL, "post_processing");
    // 配置着色器 把世界坐标 800*600 转换为 投影为 标准坐标
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                      static_cast<GLfloat>(this->Height), 0.0f,
                                      -1.0f, // 近平面
                                      1.0f); // 远平面
    ResourceManager::GetShader("sprite").use();
    ResourceManager::GetShader("sprite").setInt("image", 0);
    ResourceManager::GetShader("sprite").setMat4("projection", projection);
    ResourceManager::GetShader("particle").use();
    ResourceManager::GetShader("particle").setInt("image", 0);
    ResourceManager::GetShader("particle").setMat4("projection", projection);
    // 加载纹理
    ResourceManager::LoadTexture("img/background2.jpg", "bakcground");
    ResourceManager::LoadTexture("img/awesomeface.png", "face");
    ResourceManager::LoadTexture("img/block.png", "block");
    ResourceManager::LoadTexture("img/block_solid.png", "block_solid");
    ResourceManager::LoadTexture("img/paddle.png", "paddle");
    ResourceManager::LoadTexture("img/particle.png", "particle");
    ResourceManager::LoadTexture("img/powerup_speed.png", "powerup_speed");
    ResourceManager::LoadTexture("img/powerup_sticky.png", "powerup_sticky");
    ResourceManager::LoadTexture("img/powerup_increase.png", "powerup_increase");
    ResourceManager::LoadTexture("img/powerup_confuse.png", "powerup_confuse");
    ResourceManager::LoadTexture("img/powerup_chaos.png", "powerup_chaos");
    ResourceManager::LoadTexture("img/powerup_passthrough.png", "powerup_passthrough");

    // 渲染控制
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"),
                                      ResourceManager::GetTexture("particle"),
                                      500); // 粒子
    Effects = new PostProcessor(ResourceManager::GetShader("post_processing"), this->Width, this->Height);
    // 加载关卡
    GameLevel one, two, three, four;;
    one.Load("level/one", this->Width, this->Height * 0.5);
    two.Load("level/two", this->Width, this->Height * 0.5);
    three.Load("level/three", this->Width, this->Height * 0.5);
    four.Load("level/four", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    // 配置球拍
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE,ResourceManager::GetTexture("paddle"));
    // 球
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, BALL_VELOCITY, ResourceManager::GetTexture("face"));
    // 初始化引擎 加载声音
//    SoundEngine = irrklang::createIrrKlangDevice();
//    SoundEngine->addSoundSourceFromFile("audio/breakout.mp3");
//    SoundEngine->play2D("audio/breakout.mp3");


}

bool last;
void Game::ProcessInput(GLfloat dt)
{
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // 移动球拍
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0)
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                {
                    Ball->Position.x -= velocity;
                }
            }

        }

        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                {
                    Ball->Position.x += velocity;
                }
            }

        }

        if (this->Keys[GLFW_KEY_SPACE])
        {
            Ball->Stuck = false;
        }

        if (this->Keys[GLFW_KEY_S])
        {
            last = true;
        }
        if (!this->Keys[GLFW_KEY_S]&&last)
        {
            last = false;
            this->Level = ++this->Level % this->Levels.size();
        }

    }
}

void Game::Update(GLfloat dt)
{
    // 移动
    Ball->Move(dt, this->Width);
    // 碰撞检测
    this->DoCollisions();
    // 粒子
    Particles->Update(dt, *Ball, 1, glm::vec2(Ball->Radius/2));
    // 道具
    this->UpdatePowerUps(dt);
    // 后期处理
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }
    // 失败检测
    if (Ball->Position.y >= this->Height)
    {
        //this->ResetLevel();
        this->ResetPlayer();
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        Effects->BeginRender(); // ---------------------------

        // Draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("bakcground"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);
        // 道具
        for (PowerUp &powerUp : this->PowerUps)
        {
            if (!powerUp.Destroyed){
                powerUp.Draw(*Renderer);
            }
        }
        // 粒子
        Particles->Draw();
        // ball
        Ball->Draw(*Renderer);

        Effects->EndRender(); // ---------------------------
        Effects->Render(glfwGetTime()); // 最终画面
    }
}

// 碰撞检测
GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB-AABB 碰撞
{
    // x轴方向碰撞？
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
            two.Position.x + two.Size.x >= one.Position.x;
    // y轴方向碰撞？
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
            two.Position.y + two.Size.y >= one.Position.y;
    return collisionX && collisionY;
}
//GLboolean checkCollision(BallObject &one, GameObject &two) // AABB-圆 碰撞
//{
//    // 获取圆的中心
//    glm::vec2 center(one.Position + one.Radius);
//    // 四边形信息 中心 半边长
//    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
//    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x,
//                          two.Position.y + aabb_half_extents.y);
//    // 获取两个中心的矢量差
//    glm::vec2 difference = center - aabb_center;
//    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
//    //float clamp (float value, float min, float max) { return std::max(min, std::min(max, value)); }
//    // 获得圆心center和最近点的矢量并判断是否 length <= radius
//    glm::vec2 closest = aabb_center + clamped;
//    difference = closest - center;
//    return glm::length(difference) < one.Radius;
//}
// 相关函数声明
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 target);
void ActivatePowerUp(PowerUp &p);
void Game::DoCollisions()
{
    // 和每一个砖块检测碰撞
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // 碰撞了
            {
                if (!box.IsSolid)
                {
                    box.Destroyed = true;
                    this->SpawnPowerUps(box); // 尝试生成道具
                }
                else
                { // 砖块是坚固的的就晃动屏幕
                    ShakeTime = 0.05f;
                    Effects->Shake = true;
                }
                // 碰撞处理
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(Ball->PassThrough && !box.IsSolid)) // 没有道具穿透效果就执行碰撞
                {
                    if (dir == LEFT || dir == RIGHT)
                    {
                        Ball->Velocity.x = -Ball->Velocity.x; // 反向速度
                        // 重定位
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.x); // 穿透了多少
                        if (dir == LEFT) {
                            Ball->Position.x += penetration;
                        } else {
                            Ball->Position.x -= penetration;
                        }
                    }
                    else // 上下方向
                    {
                        Ball->Velocity.y = -Ball->Velocity.y;
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                        if (dir == UP) {
                            Ball->Position.y -= penetration;
                        } else {
                            Ball->Position.y += penetration;
                        }
                    }
                } // end if (!(Ball->PassThrough && !box.IsSolid))
            } // end if (std::get<0>(collision))
        } // end if (!box.Destroyed)
    } // end for

    // 检测道具与球拍的碰撞 以激活道具
    for (PowerUp &powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            // 掉出屏幕就销毁
            if (powerUp.Position.y >= this->Height)
            {
                powerUp.Destroyed = true;
            }
            // 和球拍碰撞就激活并销毁
            if (CheckCollision(*Player, powerUp))
            {
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = true;
                powerUp.Activated = true;
            }
        }
    }

    // 同样要检测和球板子的碰撞
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result)) // 黏在板子的时候不检测
    {
        // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度  碰到哪边就往哪边移动
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2); // 百分比
        // 加上一个强度
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity; // 存储旧速度，不改变y轴强度
        Ball->Velocity.x = BALL_VELOCITY.x * percentage * strength;
        // 球从上到下碰到板子总是往上面弹 所以我们不翻转， 直接设置为屏幕往上的方向
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y); // 防止粘板问题

        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // 保持速度一致(乘以旧速度的长度，因此总强度不变)
        // 改变速度在不同方向的强度 ， 但总速度不变

        Ball->Stuck = Ball->Sticky; // 道具效果
    }
}

void Game::ResetLevel()
{
    // 重新加载砖块
    if (this->Level == 0)this->Levels[0].Load("level/one", this->Width, this->Height * 0.5f);
    else if (this->Level == 1)
        this->Levels[1].Load("level/two", this->Width, this->Height * 0.5f);
    else if (this->Level == 2)
        this->Levels[2].Load("level/three", this->Width, this->Height * 0.5f);
    else if (this->Level == 3)
        this->Levels[3].Load("level/four", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), BALL_VELOCITY);
}

static bool shouldSpawn(int chance)
{
    int random = ::rand() % chance;
    return random == 0;
}
void Game::SpawnPowerUps(GameObject &block)
{   // 生成道具
    if (shouldSpawn(45))
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
    if (shouldSpawn(45))
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
    if (shouldSpawn(45))
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
    if (shouldSpawn(45))
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
    if (shouldSpawn(15)) // 减益效果的道具生成几率更大
        this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
    if (shouldSpawn(15))
        this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}
static bool isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);
void Game::UpdatePowerUps(GLfloat dt)
{
    //cout << "update dt- " << dt << endl;
    for (PowerUp &powerUp : this->PowerUps)
    {
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated)
        {
            powerUp.Duration -= dt;
            if (powerUp.Duration <= 0.0f)
            {
                powerUp.Activated = false;
                // 停用效果
                if (powerUp.Type == "sticky")
                {
                    if (!isOtherPowerUpActive(this->PowerUps, "sticky")) // 没有另一个同样效果的道具激活中
                    {
                        cout <<"stop sticky"<<endl;
                        Ball->Sticky = false;
                        Player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "pass-through")
                {
                    if (!isOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {
                        Ball->PassThrough = false;
                        Ball->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "confuse")
                {
                    if (!isOtherPowerUpActive(this->PowerUps, "confuse"))
                    {
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp.Type == "chaos")
                {
                    if (!isOtherPowerUpActive(this->PowerUps, "chaos"))
                    {
                        Effects->Chaos = false;
                    }
                }
            } // end if (powerUp.Duration <= 0.0f)
        } // end if (powerUp.Activated)
    } // for

    // 移除道具无用和未激活道具
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
                                        [](const PowerUp &p){ return p.Destroyed && !p.Activated; }),
                         this->PowerUps.end());
}

Collision CheckCollision(BallObject &one, GameObject &two)
{
    // 获取圆的中心
    glm::vec2 center(one.Position + one.Radius);
    // 四边形信息 中心 半边长
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x,
                          two.Position.y + aabb_half_extents.y);
    // 获取两个中心的矢量差
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    //float clamp (float value, float min, float max) { return std::max(min, std::min(max, value)); }
    // 获得圆心center和最近点的矢量并判断是否 length <= radius
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;

    if (glm::length(difference) < one.Radius) // 发生碰撞
    {
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else
    {
        return std::make_tuple(false, UP, glm::vec2(0,0));
    }
}

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = { // 罗盘 对应 头文件中的方向枚举
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    GLfloat max = 0.0f;  // 和某个方向向量点乘越接近1, 夹角就越小
    GLuint best_match = -1; //0xff..
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

static bool isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return true;
    }
    return false;
}

void ActivatePowerUp(PowerUp &p)
{
    // 激活道具效果
    if (p.Type == "speed")
    {
        Ball->Velocity *= 1.2;
    }
    else if (p.Type == "sticky")
    {
        Ball->Sticky = true;
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (p.Type == "pass-through")
    {
        Ball->PassThrough = true;
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (p.Type == "pad-size-increase")
    {
        if (Player->Size.x + 50 < PLAYER_SIZE.x * 5) // 不能超过5倍长度
            Player->Size.x += 50;
    }
    else if (p.Type == "confuse")
    {
        if (!Effects->Chaos) // 防止两种后期效果冲突
            Effects->Confuse = true;
    }
    else if (p.Type == "chaos")
    {
        if (!Effects->Confuse) // 防止两种后期效果冲突
            Effects->Chaos = true;
    }
}
