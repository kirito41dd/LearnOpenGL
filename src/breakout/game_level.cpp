#include <breakout/game_level.h>

#include <fstream>
#include <sstream>
#include <ostream>

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
    this->Bricks.clear();
    GLuint tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLuint> > tileData;
    if (fstream)
    {
        while (std::getline(fstream, line))
        {
            std::istringstream sstream(line);
            std::vector<GLuint> row;
            while (sstream >> tileCode)
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
    else
    {
        std::cout << "read file " << file << " fail" << std::endl;
    }

}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (GameObject &tile : this->Bricks) // 除了坚硬的,都已经摧毁才算完成
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<GLuint> > tileData, GLuint levelWidth, GLuint levelHeight)
{
    GLuint height = tileData.size();
    GLuint width = tileData[0].size();
    GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);
    GLfloat unit_height = levelHeight / static_cast<GLfloat>(height);

    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            GLuint type = tileData[y][x];
            // 检查砖块类型
            if (type == 1) // 坚硬的
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                this->Bricks.push_back(obj);
            }
            else if (type > 1) // 非固体 现在根据级别数据确定其颜色
            {
                glm::vec3 color = glm::vec3(1.0f); // 白色为起点
                if (tileData[y][x] == 2)
                   color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                   color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                   color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                   color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
            }
        }
    }
}
