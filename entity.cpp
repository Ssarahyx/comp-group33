#include "entity.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <random>

using namespace std;

// Initialize player
Entity initPlayer(int startX, int startY) {
    Entity player;
    player.x = startX;
    player.y = startY;
    player.type = 'P';
    player.active = true;
    player.id = 0;
    return player;
}

// Initialize enemies based on difficulty
vector<Entity> initEnemies(const GameConfig& config) {
    vector<Entity> enemies;
    int nextId = 1;
    
    // Create TAs
    for (int i = 0; i < config.taCount; i++) {
        Entity ta;
        ta.type = 'T';
        ta.active = true;
        ta.id = nextId++;
        enemies.push_back(ta);
    }
    
    // Create Professors
    for (int i = 0; i < config.professorCount; i++) {
        Entity professor;
        professor.type = 'F';
        professor.active = true;
        professor.id = nextId++;
        enemies.push_back(professor);
    }
    
    // Create Students
    for (int i = 0; i < config.studentCount; i++) {
        Entity student;
        student.type = 'S';
        student.active = true;
        student.id = nextId++;
        enemies.push_back(student);
    }
    
    return enemies;
}

// Player movement (using isWalkable detection from Module B)
bool movePlayer(Entity& player, char direction, 
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight) {
    
    int newX = player.x;
    int newY = player.y;
    
    // Calculate new position based on input
    switch (direction) {
        case 'w': case 'W': newY--; break;  // Up
        case 's': case 'S': newY++; break;  // Down
        case 'a': case 'A': newX--; break;  // Left
        case 'd': case 'D': newX++; break;  // Right
        default: 
            cout << "Invalid direction! Use W/A/S/D." << endl;
            return false;  // Invalid input
    }
    
    // Check boundaries
    if (newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight) {
        cout << "Cannot move outside map boundaries!" << endl;
        return false;
    }
    
    // Use Module B's isWalkable to check if position is walkable
    if (isWalkable(newX, newY)) {
        player.x = newX;
        player.y = newY;
        return true;
    } else {
        cout << "Cannot move there! That position is blocked." << endl;
        return false;
    }
}

// Enemy movement
void moveEnemies(vector<Entity>& enemies, const Entity& player,
                function<bool(int, int)> isWalkable,
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
        
        // Different behaviors based on enemy type
        switch (enemy.type) {
            case 'T': // TA - 65% chance to chase player, 35% random movement
              {
                    int randomChoice = rand() % 100;
                    
                    if (randomChoice < 65) {  // 65% chance to chase player
                        // Calculate direction to player
                        int dx = player.x - enemy.x;
                        int dy = player.y - enemy.y;
                        
                        // Prioritize moving in direction that reduces distance the most
                        if (abs(dx) > abs(dy)) {
                            newX += (dx > 0) ? 1 : -1;
                        } else {
                            newY += (dy > 0) ? 1 : -1;
                        }
                    } else {  // 35% chance for random movement
                        int direction = rand() % 4;
                        switch (direction) {
                            case 0: newY--; break;  // Up
                            case 1: newY++; break;  // Down
                            case 2: newX--; break;  // Left
                            case 3: newX++; break;  // Right
                        }
                    }
                }
                break;
                
            case 'F':  // Professor - 100% chase player
                {
                    // Calculate direction to player
                    int dx = player.x - enemy.x;
                    int dy = player.y - enemy.y;
                    
                    // Prioritize moving in direction that reduces distance the most
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? 1 : -1;
                    } else {
                        newY += (dy > 0) ? 1 : -1;
                    }
                }
                break;
                
            case 'S':  // Student - Random movement only (no chasing)
                {
                    int direction = rand() % 4;
                    switch (direction) {
                        case 0: newY--; break;  // Up
                        case 1: newY++; break;  // Down
                        case 2: newX--; break;  // Left
                        case 3: newX++; break;  // Right
                    }
                }
                break;
        }
        
        // Enhanced position validation - ALLOW moving into player position
        if (newX >= 0 && newX < mapWidth && 
            newY >= 0 && newY < mapHeight && 
            isWalkable(newX, newY)) {
            
            // Allow enemies to move into player position (this will trigger collision)
            // Only check if position is occupied by other enemies, not player
            bool positionOccupiedByOtherEnemy = false;
            for (const auto& otherEnemy : enemies) {
                if (&otherEnemy != &enemy && otherEnemy.active && 
                    otherEnemy.x == newX && otherEnemy.y == newY) {
                    positionOccupiedByOtherEnemy = true;
                    break;
                }
            }
            
            if (!positionOccupiedByOtherEnemy) {
                enemy.x = newX;
                enemy.y = newY;
            }
        }
    }
}

// Collision detection
bool isCollide(const Entity& entity1, const Entity& entity2) {
    return (entity1.x == entity2.x && 
            entity1.y == entity2.y && 
            entity1.active && 
            entity2.active);
}

// Check if player collides with any enemy
Entity* checkPlayerCollision(const Entity& player, vector<Entity>& enemies) {
    for (auto& enemy : enemies) {
        if (isCollide(player, enemy)) {
            return &enemy;
        }
    }
    return nullptr;
}

// Deactivate enemy (called when answer is correct)
void deactivateEnemy(Entity& enemy) {
    enemy.active = false;
}

// Get entity type name
string getEntityTypeName(char type) {
    switch (type) {
        case 'P': return "Player";
        case 'T': return "TA";
        case 'F': return "Professor";
        case 'S': return "Student";
        default: return "Unknown";
    }
}
