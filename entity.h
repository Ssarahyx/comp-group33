#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

// 难度级别
enum DifficultyLevel {
    EASY = 1,
    NORMAL = 2,
    HARD = 3
};

// 实体结构体
struct Entity {
    int x;
    int y;
    char type;      // 'P'-玩家, 'T'-TA, 'F'-教授, 'S'-学生
    bool active;
    int id;
};

// 游戏配置
struct GameConfig {
    DifficultyLevel level;
    int taCount;
    int professorCount; 
    int studentCount;
    // 地图大小由B模块提供，不在这里固定
};

// 初始化玩家
Entity initPlayer(int startX, int startY);

// 初始化敌人（根据难度）
std::vector<Entity> initEnemies(const GameConfig& config);

// 玩家移动 - 使用map.cpp中的isWalkable检测
bool movePlayer(Entity& player, char direction, 
                bool (*isWalkable)(int, int),  // map.cpp提供的函数
                int mapWidth, int mapHeight);  // 地图大小

// 敌人移动 - 包含AI行为
void moveEnemies(std::vector<Entity>& enemies, const Entity& player,
                bool (*isWalkable)(int, int),   // map.cpp提供的函数
                int mapWidth, int mapHeight);   // 地图大小由B模块提供

// 碰撞检测
bool isCollide(const Entity& entity1, const Entity& entity2);

// 检查玩家是否与任何敌人碰撞
Entity* checkPlayerCollision(const Entity& player, std::vector<Entity>& enemies);

// 使敌人消失（答对时调用）
void deactivateEnemy(Entity& enemy);

//工具函数
std::string getEntityTypeName(char type);

#endif
