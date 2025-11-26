#include "entity.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

// External functions from map module
extern bool position_walkable(int row, int col);
extern bool at_exit_position(int row, int col);

// Check if position is valid for enemy movement
bool isValidEnemyPosition(int x, int y, function<bool(int, int)> isWalkable) {
    return isWalkable(x, y) && !at_exit_position(y, x);
}

// Initialize player at specified position
Entity initPlayer(int startX, int startY) {
    Entity player;
    player.x = startX;
    player.y = startY;
    player.type = 'P';
    player.active = true;
    player.id = 0;
    player.chaseProbability = 0;
    player.detectionRange = 0;
    player.movementStrategy = 0;
    player.predictiveTracking = 0;
    player.distractionFactor = 0;
    return player;
}

// Initialize enemies with behavior modifiers
vector<Entity> initEnemies(const GameConfig& config) {
    vector<Entity> enemies;
    int nextId = 1;
    
    for (int i = 0; i < config.taCount; i++) {
        Entity ta;
        ta.type = 'T';
        ta.active = true;
        ta.id = nextId++;
        ta.chaseProbability = calculateTAChaseProbability(config.level, config.stage, i);
        ta.detectionRange = calculateTADetectionRange(config.level, config.stage);
        ta.movementStrategy = calculateTAMovementStrategy(config.level, config.stage, i);
        ta.predictiveTracking = 0;
        ta.distractionFactor = 0;
        enemies.push_back(ta);
    }
    
    for (int i = 0; i < config.professorCount; i++) {
        Entity professor;
        professor.type = 'F';
        professor.active = true;
        professor.id = nextId++;
        professor.chaseProbability = calculateProfessorChaseProbability(config.level, config.stage, i);
        professor.detectionRange = calculateProfessorDetectionRange(config.level, config.stage);
        professor.movementStrategy = calculateProfessorMovementStrategy(config.level, config.stage, i);
        professor.predictiveTracking = calculateProfessorPredictiveAbility(config.level, config.stage);
        professor.distractionFactor = 0;
        enemies.push_back(professor);
    }
    
    for (int i = 0; i < config.studentCount; i++) {
        Entity student;
        student.type = 'S';
        student.active = true;
        student.id = nextId++;
        student.chaseProbability = calculateStudentChaseProbability(config.level, config.stage, i);
        student.movementStrategy = calculateStudentMovementStrategy(config.level, config.stage, i);
        student.distractionFactor = calculateStudentDistraction(config.level, config.stage);
        student.detectionRange = 2;
        student.predictiveTracking = 0;
        enemies.push_back(student);
    }
    
    return enemies;
}

// Move player with direction input
bool movePlayer(Entity& player, char direction, 
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight) {
    
    int newX = player.x;
    int newY = player.y;
    
    switch (direction) {
        case 'w': case 'W': newY--; break;
        case 's': case 'S': newY++; break;
        case 'a': case 'A': newX--; break;
        case 'd': case 'D': newX++; break;
        default: 
            cout << "Invalid direction! Use W/A/S/D." << endl;
            return false;
    }
    
    if (newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight) {
        cout << "Cannot move outside map boundaries!" << endl;
        return false;
    }
    
    if (isWalkable(newX, newY)) {
        player.x = newX;
        player.y = newY;
        return true;
    } else {
        cout << "Cannot move there! That position is blocked." << endl;
        return false;
    }
}

// Move all enemies based on their type
void moveEnemies(vector<Entity>& enemies, const Entity& player,
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight) {
    
    for (auto& enemy : enemies) {
        if (!enemy.active) continue;
        
        int newX = enemy.x;
        int newY = enemy.y;
        bool shouldMove = false;
        
        int dx = player.x - enemy.x;
        int dy = player.y - enemy.y;
        int distance = abs(dx) + abs(dy);
        
        switch (enemy.type) {
            case 'T': 
                shouldMove = moveTA(enemy, player, newX, newY, distance);
                break;
            case 'F': 
                shouldMove = moveProfessor(enemy, player, newX, newY, distance);
                break;
            case 'S': 
                shouldMove = moveStudent(enemy, player, newX, newY, distance);
                break;
        }
        
        if (shouldMove && newX >= 0 && newX < mapWidth && 
            newY >= 0 && newY < mapHeight && 
            isValidEnemyPosition(newX, newY, isWalkable)) {
            
            bool positionOccupied = false;
            for (const auto& other : enemies) {
                if (&other != &enemy && other.active && 
                    other.x == newX && other.y == newY) {
                    positionOccupied = true;
                    break;
                }
            }
            
            if (!positionOccupied) {
                enemy.x = newX;
                enemy.y = newY;
            }
        }
    }
}

// TA movement with strategic chasing
bool moveTA(Entity& ta, const Entity& player, int& newX, int& newY, int distance) {
    int randomChoice = rand() % 100;
    int dx = player.x - ta.x;
    int dy = player.y - ta.y;
    
    if (distance <= ta.detectionRange && randomChoice < ta.chaseProbability) {
        switch (ta.movementStrategy) {
            case 0: 
                if (abs(dx) > abs(dy)) {
                    newX += (dx > 0) ? 1 : -1;
                } else {
                    newY += (dy > 0) ? 1 : -1;
                }
                break;
            case 1: 
                if (distance > 2) {
                    if (dx != 0) newX += (dx > 0) ? 1 : -1;
                    if (dy != 0 && rand() % 2 == 0) newY += (dy > 0) ? 1 : -1;
                } else {
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? 1 : -1;
                    } else {
                        newY += (dy > 0) ? 1 : -1;
                    }
                }
                break;
            case 2: 
                if (dx != 0) newX += (dx > 0) ? 1 : -1;
                if (dy != 0) newY += (dy > 0) ? 1 : -1;
                break;
        }
        return true;
    } else {
        int direction = rand() % 4;
        switch (direction) {
            case 0: newY--; break;
            case 1: newY++; break;
            case 2: newX--; break;
            case 3: newX++; break;
        }
        return true;
    }
}

// Professor movement with advanced tracking - always chase
bool moveProfessor(Entity& professor, const Entity& player, int& newX, int& newY, int distance) {
    int dx = player.x - professor.x;
    int dy = player.y - professor.y;

    
    if (distance <= professor.detectionRange) {
        switch (professor.movementStrategy) {
            case 0: 
                if (abs(dx) > abs(dy)) {
                    newX += (dx > 0) ? 1 : -1;
                } else {
                    newY += (dy > 0) ? 1 : -1;
                }
                break;
            case 1: 
                if (distance > 3) {
                    if (dx != 0) newX += (dx > 0) ? 1 : -1;
                    if (dy != 0) newY += (dy > 0) ? 1 : -1;
                } else {
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? 1 : -1;
                    } else {
                        newY += (dy > 0) ? 1 : -1;
                    }
                }
                break;
            case 2: 
                if (distance < 2) {
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? -1 : 1;
                    } else {
                        newY += (dy > 0) ? -1 : 1;
                    }
                } else {
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? 1 : -1;
                    } else {
                        newY += (dy > 0) ? 1 : -1;
                    }
                }
                break;
        }
        return true;
    } else {
        if (dx != 0) newX += (dx > 0) ? 1 : -1;
        if (dy != 0) newY += (dy > 0) ? 1 : -1;
        return true;
    }
}
// Student movement with random behaviors
bool moveStudent(Entity& student, const Entity& player, int& newX, int& newY, int distance) {
    int randomChoice = rand() % 100;
    
    if (distance <= 3 && randomChoice < student.chaseProbability) {
        int dx = player.x - student.x;
        int dy = player.y - student.y;
        if (abs(dx) > abs(dy)) {
            newX += (dx > 0) ? 1 : -1;
        } else {
            newY += (dy > 0) ? 1 : -1;
        }
        return true;
    } else {
        switch (student.movementStrategy) {
            case 0: {
                int direction = rand() % 4;
                switch (direction) {
                    case 0: newY--; break;
                    case 1: newY++; break;
                    case 2: newX--; break;
                    case 3: newX++; break;
                }
                break;
            }
            case 1: 
                if (rand() % 100 < student.distractionFactor) {
                    int direction = rand() % 4;
                    int steps = 1 + (rand() % 2);
                    switch (direction) {
                        case 0: newY -= steps; break;
                        case 1: newY += steps; break;
                        case 2: newX -= steps; break;
                        case 3: newX += steps; break;
                    }
                } else {
                    int direction = rand() % 4;
                    switch (direction) {
                        case 0: newY--; break;
                        case 1: newY++; break;
                        case 2: newX--; break;
                        case 3: newX++; break;
                    }
                }
                break;
            case 2: 
                if (distance < 4) {
                    int dx = player.x - student.x;
                    int dy = player.y - student.y;
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? -1 : 1;
                    } else {
                        newY += (dy > 0) ? -1 : 1;
                    }
                } else {
                    int direction = rand() % 4;
                    switch (direction) {
                        case 0: newY--; break;
                        case 1: newY++; break;
                        case 2: newX--; break;
                        case 3: newX++; break;
                    }
                }
                break;
        }
        return true;
    }
}

// Check if two entities are colliding
bool isCollide(const Entity& entity1, const Entity& entity2) {
    return (entity1.x == entity2.x && 
            entity1.y == entity2.y && 
            entity1.active && 
            entity2.active);
}

// Check if player collides with any enemy
Entity* checkPlayerCollision(const Entity& player, vector<Entity>& enemies) {
    for (auto& enemy : enemies) {
        if (!enemy.active) continue;
        if (isCollide(player, enemy)) {
            return &enemy;
        }
    }
    return nullptr;
}

// Deactivate enemy when question is answered correctly
void deactivateEnemy(Entity& enemy) {
    enemy.active = false;
}

// Get readable name for entity type
string getEntityTypeName(char type) {
    switch (type) {
        case 'P': return "Player";
        case 'T': return "TA";
        case 'F': return "Professor";
        case 'S': return "Student";
        default: return "Unknown";
    }
}

// Calculate TA chase probability
int calculateTAChaseProbability(int level, int stage, int enemyIndex) {
    int baseProbability = 60;
    if (level == 1) baseProbability += 5;
    else if (level == 2) baseProbability += 15;
    else baseProbability += 25;
    baseProbability += stage * 3;
    baseProbability += (enemyIndex % 3) * 2;
    return min(85, baseProbability);
}

// Calculate TA detection range
int calculateTADetectionRange(int level, int stage) {
    return 3 + level + (stage > 1 ? 1 : 0);
}

// Determine TA movement strategy
int calculateTAMovementStrategy(int level, int stage, int enemyIndex) {
    return (level + stage + enemyIndex) % 3;
}

// Calculate Professor chase probability
int calculateProfessorChaseProbability(int level, int stage, int enemyIndex) {
    int baseProbability = 75;
    if (level == 1) baseProbability += 10;
    else if (level == 2) baseProbability += 20;
    else baseProbability += 30;
    baseProbability += stage * 5;
    if (enemyIndex == 0) baseProbability += 5;
    return min(95, baseProbability);
}

// Calculate Professor detection range
int calculateProfessorDetectionRange(int level, int stage) {
    return 4 + level * 2 + (stage > 1 ? 1 : 0);
}

// Determine Professor movement strategy
int calculateProfessorMovementStrategy(int level, int stage, int enemyIndex) {
    return (level * 2 + stage + enemyIndex) % 3;
}

// Determine Professor predictive ability
int calculateProfessorPredictiveAbility(int level, int stage) {
    return level > 1 && stage > 1 ? 1 : 0;
}

// Calculate Student chase probability
int calculateStudentChaseProbability(int level, int stage, int enemyIndex) {
    int baseProbability = 20;
    baseProbability += level * 5;
    baseProbability += stage * 2;
    baseProbability += (enemyIndex % 2) * 10;
    return min(50, baseProbability);
}

// Determine Student movement strategy
int calculateStudentMovementStrategy(int level, int stage, int enemyIndex) {
    return (level + stage * 2 + enemyIndex) % 3;
}

// Calculate Student distraction factor
int calculateStudentDistraction(int level, int stage) {
    return 30 - (stage * 3) + (level * 2);
}
