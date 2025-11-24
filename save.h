#ifndef SAVE_H
#define SAVE_H

#include <string>
#include <vector>

using namespace std;

// 统一难度级别定义
enum DifficultyLevelEnum {
    EASY = 1,
    NORMAL = 2, 
    HARD = 3
};

// 统一实体结构定义
struct Entity {
    int x;
    int y;
    char type;
    bool active;
    int id;
};

// 游戏难度设置
struct GameDifficultySettings {
    string name;
    double initialGPA;
    double ta_k;
    double prof_k;
    double stu_k;
};

// 函数声明
GameDifficultySettings easy();
GameDifficultySettings normal();
GameDifficultySettings hard();
bool saveGame(int level, double gpa, const Entity& player, const vector<Entity>& enemies, const GameDifficultySettings& diff);
bool loadGame(int& level, double& gpa, Entity& player, vector<Entity>& enemies, GameDifficultySettings& diff);

#endif
