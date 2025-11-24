#include <vector>
#include <string>
#include <functional>
#include "save.h" 

// 难度级别
enum DifficultyLevel {
@@ -22,7 +24,7 @@ struct Entity {

// 游戏配置
struct GameConfig {
    DifficultyLevel level;
    int level;
    int taCount;
    int professorCount; 
    int studentCount;
@@ -33,28 +35,28 @@ struct GameConfig {
Entity initPlayer(int startX, int startY);

// 初始化敌人（根据难度）
std::vector<Entity> initEnemies(const GameConfig& config);
vector<Entity> initEnemies(const GameConfig& config);

// 玩家移动 - 使用map.cpp中的isWalkable检测
bool movePlayer(Entity& player, char direction, 
                bool (*isWalkable)(int, int),  // map.cpp提供的函数
                int mapWidth, int mapHeight);  // 地图大小
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight);

// 敌人移动 - 包含AI行为
void moveEnemies(std::vector<Entity>& enemies, const Entity& player,
                bool (*isWalkable)(int, int),   // map.cpp提供的函数
                int mapWidth, int mapHeight);   // 地图大小由B模块提供
// 敌人移动
void moveEnemies(vector<Entity>& enemies, const Entity& player,
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight);

// 碰撞检测
bool isCollide(const Entity& entity1, const Entity& entity2);

// 检查玩家是否与任何敌人碰撞
Entity* checkPlayerCollision(const Entity& player, std::vector<Entity>& enemies);
Entity* checkPlayerCollision(const Entity& player, vector<Entity>& enemies);

// 使敌人消失（答对时调用）
void deactivateEnemy(Entity& enemy);

//工具函数
std::string getEntityTypeName(char type);
string getEntityTypeName(char type);

#endif
