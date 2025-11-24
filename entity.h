#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <functional>
#include "save.h" 

using namespace std;

// Game configuration
struct GameConfig {
    int level;
    int taCount;
    int professorCount; 
    int studentCount;
};  // Fixed: added missing semicolon

// Entity structure definition (assuming it's defined elsewhere, likely in save.h)
// If not, you need to add it here:
struct Entity {
    int x;
    int y;
    char type;
    bool active;
    int id;
};

// Initialize player
Entity initPlayer(int startX, int startY);

// Initialize enemies based on difficulty
vector<Entity> initEnemies(const GameConfig& config);

// Player movement - using isWalkable detection from map.cpp
bool movePlayer(Entity& player, char direction, 
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight);

// Enemy movement
void moveEnemies(vector<Entity>& enemies, const Entity& player,
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight);

// Collision detection
bool isCollide(const Entity& entity1, const Entity& entity2);

// Check if player collides with any enemy
Entity* checkPlayerCollision(const Entity& player, vector<Entity>& enemies);

// Deactivate enemy (called when answer is correct)
void deactivateEnemy(Entity& enemy);

// Utility functions
string getEntityTypeName(char type);

#endif
