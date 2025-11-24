#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <functional>
#include "save.h" 

using namespace std;

// 游戏配置
struct GameConfig {
    int level;
    int taCount;
    int professorCount; 
    int studentCount;
}

// 初始化敌人（根据难度）
Entity initPlayer(int startX, int startY);
vector<Entity> initEnemies(const GameConfig& config);

// 玩家移动 - 使用map.cpp中的isWalkable检测
bool movePlayer(Entity& player, char direction, 
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight);

// 敌人移动
void moveEnemies(vector<Entity>& enemies, const Entity& player,
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight);

// 碰撞检测
bool isCollide(const Entity& entity1, const Entity& entity2);

// 检查玩家是否与任何敌人碰撞
Entity* checkPlayerCollision(const Entity& player, vector<Entity>& enemies);

// 使敌人消失（答对时调用）
void deactivateEnemy(Entity& enemy);

//工具函数
string getEntityTypeName(char type);

#endif
