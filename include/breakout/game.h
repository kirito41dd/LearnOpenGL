#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <breakout/game_level.h>
#include <breakout/power_up.h>

#include <tuple>

// 玩家球拍的初始尺寸 初始速度
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
// 球的初始化半径和速度
const glm::vec2 BALL_VELOCITY(100.0f, -305.0f);
const GLfloat BALL_RADIUS = 12.5f;

// 游戏当前状态
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};
// 方向
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// 碰撞返回类型
typedef std::tuple<bool, Direction, glm::vec2> Collision; // 是否碰撞 方向 difference向量(closest - center)

// 游戏类拥有所有与游戏相关的状态和功能。
class Game
{
public:

    GameState   State;
    GLboolean   Keys[1024];
    GLuint      Width, Height;
    std::vector<GameLevel>  Levels;
    GLuint                  Level;
    std::vector<PowerUp>    PowerUps;

    Game(GLuint width, GLuint height);
    ~Game();

    // 初始化游戏状态，加载所有 着色器 纹理 关卡
    void Init();

    // 游戏循环
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    // 执行碰撞检测
    void DoCollisions();

    // 重置
    void ResetLevel();
    void ResetPlayer();
    // 道具
    void SpawnPowerUps(GameObject &block);
    void UpdatePowerUps(GLfloat dt);

}; // class Game

#endif // GAME_H
