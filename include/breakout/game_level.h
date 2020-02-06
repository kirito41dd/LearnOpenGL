#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <breakout/game_object.h>
#include <breakout/sprite_renderer.h>
#include <breakout/resource_manager.h>

#include <vector>

class GameLevel
{
public:
    // 关卡状态
    std::vector<GameObject> Bricks; // 砖块

    GameLevel() = default;
    // 加载关卡
    void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
    // 渲染关卡
    void Draw(SpriteRenderer &renderer);
    // 检查关卡是否完成
    bool IsCompleted();
private:
    // 初始化关卡  通过砖块数据
     void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);

}; // class GameLevel

#endif // GAME_LEVEL_H
