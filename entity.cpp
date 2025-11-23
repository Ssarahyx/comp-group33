#include "entity.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <random>

using namespace std;

// 初始化玩家
Entity initPlayer(int startX, int startY) {
    Entity player;
    player.x = startX;
    player.y = startY;
    player.type = 'P';
    player.active = true;
    player.id = 0;
    return player;
}

// 初始化敌人（根据难度）
vector<Entity> initEnemies(const GameConfig& config) {
    vector<Entity> enemies;
    int nextId = 1;
    
    // 创建TA
    for (int i = 0; i < config.taCount; i++) {
        Entity ta;
        ta.type = 'T';
        ta.active = true;
        ta.id = nextId++;
        enemies.push_back(ta);
    }
    
    // 创建教授
    for (int i = 0; i < config.professorCount; i++) {
        Entity professor;
        professor.type = 'F';
        professor.active = true;
        professor.id = nextId++;
        enemies.push_back(professor);
    }
    
    // 创建学生
    for (int i = 0; i < config.studentCount; i++) {
        Entity student;
        student.type = 'S';
        student.active = true;
        student.id = nextId++;
        enemies.push_back(student);
    }
    
    return enemies;
}

// 玩家移动（使用B模块的isWalkable检测）
bool movePlayer(Entity& player, char direction, 
                bool (*isWalkable)(int, int),
                int mapWidth, int mapHeight) {
    
    int newX = player.x;
    int newY = player.y;
    
    // 根据输入计算新位置
    switch (direction) {
        case 'w': case 'W': newY--; break;  // 上
        case 's': case 'S': newY++; break;  // 下  
        case 'a': case 'A': newX--; break;  // 左
        case 'd': case 'D': newX++; break;  // 右
        default: return false;  // 无效输入
    }
    
    // 检查边界
    if (newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight) {
        return false;
    }
    
    // 使用B模块的isWalkable检测是否可走
    if (isWalkable(newX, newY)) {
        player.x = newX;
        player.y = newY;
        return true;
    }
    
    return false;
}

// 敌人移动
void moveEnemies(vector<Entity>& enemies, const Entity& player,
                bool (*isWalkable)(int, int),
                int mapWidth, int mapHeight) {
    
    static bool seeded = false;
    if (!seeded) {
        srand(time(nullptr));
        seeded = true;
    }
    
    for (auto& enemy : enemies) {
        if (!enemy.active) continue;
        
        int newX = enemy.x;
        int newY = enemy.y;
        
        // 根据敌人类型选择不同行为
        switch (enemy.type) {
            case 'T': // TA - 65%概率追踪玩家，35%概率随机移动
              {
                    int randomChoice = rand() % 100;
                    
                    if (randomChoice < 65) {  // 65%概率追踪玩家
                        // 计算与玩家的方向
                        int dx = player.x - enemy.x;
                        int dy = player.y - enemy.y;
                        
                        // 优先移动能减少距离的方向
                        if (abs(dx) > abs(dy)) {
                            newX += (dx > 0) ? 1 : -1;
                        } else {
                            newY += (dy > 0) ? 1 : -1;
                        }
                    } else {  // 35%概率随机移动
                        int direction = rand() % 4;
                        switch (direction) {
                            case 0: newY--; break;  // 上
                            case 1: newY++; break;  // 下
                            case 2: newX--; break;  // 左
                            case 3: newX++; break;  // 右
                        }
                    }
                }
                break;
                
            case 'F':  // 教授 - 100%追踪玩家
                {
                    // 计算与玩家的方向
                    int dx = player.x - enemy.x;
                    int dy = player.y - enemy.y;
                    
                    // 优先移动能减少距离的方向
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? 1 : -1;
                    } else {
                        newY += (dy > 0) ? 1 : -1;
                    }
                }
                break;
                
            case 'S':  // 同学 - 随机移动（满足随机事件要求）
                {
                    int direction = rand() % 4;
                    switch (direction) {
                        case 0: newY--; break;  // 上
                        case 1: newY++; break;  // 下
                        case 2: newX--; break;  // 左
                        case 3: newX++; break;  // 右
                    }
                }
                break;
        }
        
        // 检查边界和可走性
        if (newX >= 0 && newX < mapWidth && 
            newY >= 0 && newY < mapHeight && 
            isWalkable(newX, newY)) {
            enemy.x = newX;
            enemy.y = newY;
        }
    }
}

// 碰撞检测
bool isCollide(const Entity& entity1, const Entity& entity2) {
    return (entity1.x == entity2.x && 
            entity1.y == entity2.y && 
            entity1.active && 
            entity2.active);
}

// 检查玩家是否与任何敌人碰撞
Entity* checkPlayerCollision(const Entity& player, vector<Entity>& enemies) {
    for (auto& enemy : enemies) {
        if (isCollide(player, enemy)) {
            return &enemy;
        }
    }
    return nullptr;
}

// 使敌人消失（答对时调用）
void deactivateEnemy(Entity& enemy) {
    enemy.active = false;
}

// 获取实体类型名称
string getEntityTypeName(char type) {
    switch (type) {
        case 'P': return "玩家";
        case 'T': return "TA";
        case 'F': return "教授";
        case 'S': return "学生";
        default: return "未知";
    }
}
