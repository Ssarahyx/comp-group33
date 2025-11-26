#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <functional>
#include "save.h"

using namespace std;

// Game configuration structure
struct GameConfig {
    int level;           // Difficulty level (1-3)
    int stage;           // Stage (1-3)
    int taCount;
    int professorCount;
    int studentCount;
};

// Initialize player at specified position
Entity initPlayer(int startX, int startY);

// Initialize enemies based on difficulty configuration
vector<Entity> initEnemies(const GameConfig& config);

// Move player in specified direction with collision checking
bool movePlayer(Entity& player, char direction, 
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight);

// Move all enemies based on their behavior patterns
void moveEnemies(vector<Entity>& enemies, const Entity& player,
                 function<bool(int, int)> isWalkable,
                 int mapWidth, int mapHeight);

// Check if two entities are colliding
bool isCollide(const Entity& entity1, const Entity& entity2);

// Check if player collides with any active enemy
Entity* checkPlayerCollision(const Entity& player, vector<Entity>& enemies);

// Deactivate enemy when question is answered correctly
void deactivateEnemy(Entity& enemy);

// Get readable name for entity type
string getEntityTypeName(char type);

// Check if position is valid for enemy movement
bool isValidEnemyPosition(int x, int y, function<bool(int, int)> isWalkable);

// Enemy movement helper functions
bool moveTA(Entity& ta, const Entity& player, int& newX, int& newY, int distance);
bool moveProfessor(Entity& professor, const Entity& player, int& newX, int& newY, int distance);
bool moveStudent(Entity& student, const Entity& player, int& newX, int& newY, int distance);

// Behavior calculation functions
int calculateTAChaseProbability(int level, int stage, int enemyIndex);
int calculateTADetectionRange(int level, int stage);
int calculateTAMovementStrategy(int level, int stage, int enemyIndex);
int calculateProfessorChaseProbability(int level, int stage, int enemyIndex);
int calculateProfessorDetectionRange(int level, int stage);
int calculateProfessorMovementStrategy(int level, int stage, int enemyIndex);
int calculateProfessorPredictiveAbility(int level, int stage);
int calculateStudentChaseProbability(int level, int stage, int enemyIndex);
int calculateStudentMovementStrategy(int level, int stage, int enemyIndex);
int calculateStudentDistraction(int level, int stage);

#endif
