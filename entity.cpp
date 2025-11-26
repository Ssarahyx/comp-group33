#include "entity.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>

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

// Enhanced enemy initialization with behavior modifiers
vector<Entity> initEnemies(const GameConfig& config) {
    vector<Entity> enemies;
    int nextId = 1;
    
    // Create TAs with enhanced behavior profiles
    for (int i = 0; i < config.taCount; i++) {
        Entity ta;
        ta.type = 'T';
        ta.active = true;
        ta.id = nextId++;
        
        // TA behavior modifiers based on difficulty and stage
        ta.chaseProbability = calculateTAChaseProbability(config.level, config.stage, i);
        ta.detectionRange = calculateTADetectionRange(config.level, config.stage);
        ta.movementStrategy = calculateTAMovementStrategy(config.level, config.stage, i);
        
        enemies.push_back(ta);
    }
    
    // Create Professors with advanced tracking capabilities
    for (int i = 0; i < config.professorCount; i++) {
        Entity professor;
        professor.type = 'F';
        professor.active = true;
        professor.id = nextId++;
        
        // Professor behavior modifiers
        professor.chaseProbability = calculateProfessorChaseProbability(config.level, config.stage, i);
        professor.detectionRange = calculateProfessorDetectionRange(config.level, config.stage);
        professor.movementStrategy = calculateProfessorMovementStrategy(config.level, config.stage, i);
        professor.predictiveTracking = calculateProfessorPredictiveAbility(config.level, config.stage);
        
        enemies.push_back(professor);
    }
    
    // Create Students with varied movement patterns
    for (int i = 0; i < config.studentCount; i++) {
        Entity student;
        student.type = 'S';
        student.active = true;
        student.id = nextId++;
        
        // Student behavior modifiers
        student.chaseProbability = calculateStudentChaseProbability(config.level, config.stage, i);
        student.movementStrategy = calculateStudentMovementStrategy(config.level, config.stage, i);
        student.distractionFactor = calculateStudentDistraction(config.level, config.stage);
        
        enemies.push_back(student);
    }
    
    cout << "Enhanced enemies initialized with complex behaviors" << endl;
    return enemies;
}

// Complex probability calculations for different enemy types

int calculateTAChaseProbability(int level, int stage, int enemyIndex) {
    // Base probability increases with difficulty
    int baseProbability = 60;
    
    // Level modifiers
    if (level == 1) baseProbability += 5;  // Easy
    else if (level == 2) baseProbability += 15; // Normal
    else baseProbability += 25; // Hard
    
    // Stage progression
    baseProbability += stage * 3;
    
    // Individual variation
    baseProbability += (enemyIndex % 3) * 2;
    
    return min(85, baseProbability);
}

int calculateTADetectionRange(int level, int stage) {
    return 3 + level + (stage > 1 ? 1 : 0);
}

int calculateTAMovementStrategy(int level, int stage, int enemyIndex) {
    // 0: Standard chase, 1: Strategic positioning, 2: Aggressive pursuit
    return (level + stage + enemyIndex) % 3;
}

int calculateProfessorChaseProbability(int level, int stage, int enemyIndex) {
    // Professors are highly persistent
    int baseProbability = 75;
    
    // Significant level scaling
    if (level == 1) baseProbability += 10;
    else if (level == 2) baseProbability += 20;
    else baseProbability += 30;
    
    // Stage progression makes professors more determined
    baseProbability += stage * 5;
    
    // Some professors are exceptionally persistent
    if (enemyIndex == 0) baseProbability += 5;
    
    return min(95, baseProbability);
}

int calculateProfessorDetectionRange(int level, int stage) {
    return 4 + level * 2;
}

int calculateProfessorMovementStrategy(int level, int stage, int enemyIndex) {
    // 0: Direct pursuit, 1: Flanking, 2: Area control
    return (level * 2 + stage) % 3;
}

int calculateProfessorPredictiveAbility(int level, int stage) {
    // Professors can predict player movement in higher difficulties
    return level > 1 && stage > 1 ? 1 : 0;
}

int calculateStudentChaseProbability(int level, int stage, int enemyIndex) {
    // Students are less likely to chase
    int baseProbability = 20;
    
    // Mild level scaling
    baseProbability += level * 5;
    
    // Stage progression
    baseProbability += stage * 2;
    
    // Some students are more curious
    baseProbability += (enemyIndex % 2) * 10;
    
    return min(50, baseProbability);
}

int calculateStudentMovementStrategy(int level, int stage, int enemyIndex) {
    // 0: Random wander, 1: Group tendency, 2: Avoidance
    return (level + stage * 2 + enemyIndex) % 3;
}

int calculateStudentDistraction(int level, int stage) {
    // Students get distracted, less so in higher stages
    return 30 - (stage * 3);
}

// Enhanced player movement
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
    
    // Boundary checking
    if (newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight) {
        cout << "Cannot move outside map boundaries!" << endl;
        return false;
    }
    
    // Walkability check
    if (isWalkable(newX, newY)) {
        player.x = newX;
        player.y = newY;
        return true;
    } else {
        cout << "Cannot move there! That position is blocked." << endl;
        return false;
    }
}

// Complex enemy movement with varied behaviors
void moveEnemies(vector<Entity>& enemies, const Entity& player,
                function<bool(int, int)> isWalkable,
                int mapWidth, int mapHeight) {
    
    for (auto& enemy : enemies) {
        if (!enemy.active) continue;
        
        int newX = enemy.x;
        int newY = enemy.y;
        bool shouldMove = false;
        
        // Calculate distance to player
        int dx = player.x - enemy.x;
        int dy = player.y - enemy.y;
        int distance = abs(dx) + abs(dy);
        
        // Different behaviors based on enemy type and strategy
        switch (enemy.type) {
            case 'T': // TA with complex chasing logic
                shouldMove = moveTA(enemy, player, newX, newY, distance);
                break;
                
            case 'F': // Professor with advanced tracking
                shouldMove = moveProfessor(enemy, player, newX, newY, distance);
                break;
                
            case 'S': // Student with varied movement
                shouldMove = moveStudent(enemy, player, newX, newY, distance);
                break;
        }
        
        // Validate and apply movement
        if (shouldMove && newX >= 0 && newX < mapWidth && 
            newY >= 0 && newY < mapHeight && 
            isWalkable(newX, newY)) {
            
            // Check for other enemies in target position
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

// TA movement with strategic behavior
bool moveTA(Entity& ta, const Entity& player, int& newX, int& newY, int distance) {
    int randomChoice = rand() % 100;
    int dx = player.x - ta.x;
    int dy = player.y - ta.y;
    
    // Only chase if player is within detection range
    if (distance <= ta.detectionRange && randomChoice < ta.chaseProbability) {
        // Strategic chasing based on movement strategy
        switch (ta.movementStrategy) {
            case 0: // Standard chase
                if (abs(dx) > abs(dy)) {
                    newX += (dx > 0) ? 1 : -1;
                } else {
                    newY += (dy > 0) ? 1 : -1;
                }
                break;
                
            case 1: // Strategic positioning - try to cut off player
                if (distance > 2) {
                    // Move toward player but with offset
                    if (dx != 0) newX += (dx > 0) ? 1 : -1;
                    if (dy != 0 && rand() % 2 == 0) newY += (dy > 0) ? 1 : -1;
                } else {
                    // Close range - direct approach
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? 1 : -1;
                    } else {
                        newY += (dy > 0) ? 1 : -1;
                    }
                }
                break;
                
            case 2: // Aggressive pursuit - always close distance
                if (dx != 0) newX += (dx > 0) ? 1 : -1;
                if (dy != 0) newY += (dy > 0) ? 1 : -1;
                break;
        }
        return true;
    } else {
        // Random movement when not chasing
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

// Professor movement with predictive tracking
bool moveProfessor(Entity& professor, const Entity& player, int& newX, int& newY, int distance) {
    int randomChoice = rand() % 100;
    int dx = player.x - professor.x;
    int dy = player.y - professor.y;
    
    // Professors are highly persistent chasers
    if (distance <= professor.detectionRange && randomChoice < professor.chaseProbability) {
        switch (professor.movementStrategy) {
            case 0: // Direct pursuit
                if (abs(dx) > abs(dy)) {
                    newX += (dx > 0) ? 1 : -1;
                } else {
                    newY += (dy > 0) ? 1 : -1;
                }
                break;
                
            case 1: // Flanking maneuver
                if (distance > 3) {
                    // Move diagonally toward player
                    if (dx != 0) newX += (dx > 0) ? 1 : -1;
                    if (dy != 0) newY += (dy > 0) ? 1 : -1;
                } else {
                    // Close range direct approach
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? 1 : -1;
                    } else {
                        newY += (dy > 0) ? 1 : -1;
                    }
                }
                break;
                
            case 2: // Area control - try to maintain optimal distance
                if (distance < 2) {
                    // Too close, maintain distance
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? -1 : 1;
                    } else {
                        newY += (dy > 0) ? -1 : 1;
                    }
                } else {
                    // Move toward player
                    if (abs(dx) > abs(dy)) {
                        newX += (dx > 0) ? 1 : -1;
                    } else {
                        newY += (dy > 0) ? 1 : -1;
                    }
                }
                break;
        }
        return true;
    }
    return false; // Professors rarely move randomly
}

// Student movement with social behaviors
bool moveStudent(Entity& student, const Entity& player, int& newX, int& newY, int distance) {
    int randomChoice = rand() % 100;
    
    // Students occasionally chase, but mostly wander
    if (distance <= 3 && randomChoice < student.chaseProbability) {
        int dx = player.x - student.x;
        int dy = player.y - student.y;
        
        // Simple chase logic for students
        if (abs(dx) > abs(dy)) {
            newX += (dx > 0) ? 1 : -1;
        } else {
            newY += (dy > 0) ? 1 : -1;
        }
        return true;
    } else {
        // Random movement with strategy variations
        switch (student.movementStrategy) {
            case 0: // Pure random
                {
                    int direction = rand() % 4;
                    switch (direction) {
                        case 0: newY--; break;
                        case 1: newY++; break;
                        case 2: newX--; break;
                        case 3: newX++; break;
                    }
                }
                break;
                
            case 1: // Small cluster movement - tendency to stay in groups
                if (rand() % 100 < student.distractionFactor) {
                    // Occasionally make a larger move
                    int direction = rand() % 4;
                    int steps = 1 + (rand() % 2);
                    switch (direction) {
                        case 0: newY -= steps; break;
                        case 1: newY += steps; break;
                        case 2: newX -= steps; break;
                        case 3: newX += steps; break;
                    }
                } else {
                    // Small movements
                    int direction = rand() % 4;
                    switch (direction) {
                        case 0: newY--; break;
                        case 1: newY++; break;
                        case 2: newX--; break;
                        case 3: newX++; break;
                    }
                }
                break;
                
            case 2: // Avoidance behavior - tends to move away from player
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
