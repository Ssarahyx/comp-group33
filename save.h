#ifndef SAVE_H
#define SAVE_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief Structure representing a game entity (player or enemy)
 * @param x X-coordinate position on game grid
 * @param y Y-coordinate position on game grid  
 * @param type Character representing entity type ('P'=Player, 'T'=TA, 'F'=Professor, 'S'=Student)
 */
struct Entity {
    int x;
    int y;
    char type;
};

/**
 * @brief Structure representing game difficulty level settings
 * @param name Difficulty level name (EASY, NORMAL, HARD)
 * @param initialGPA Starting GPA value for this difficulty
 * @param ta_k Penalty multiplier for TA encounters
 * @param prof_k Penalty multiplier for Professor encounters
 * @param stu_k Penalty multiplier for Student encounters
 */
struct DifficultyLevel {
    string name;
    double initialGPA;
    double ta_k;
    double prof_k;
    double stu_k;
};

// Function declarations

/**
 * @brief Creates and returns EASY difficulty settings
 * @return DifficultyLevel structure with EASY configuration
 */
DifficultyLevel easy();

/**
 * @brief Creates and returns NORMAL difficulty settings  
 * @return DifficultyLevel structure with NORMAL configuration
 */
DifficultyLevel normal();

/**
 * @brief Creates and returns HARD difficulty settings
 * @return DifficultyLevel structure with HARD configuration
 */
DifficultyLevel hard();

/**
 * @brief Saves current game state to file
 * @param level Current game level number
 * @param gpa Current player GPA value
 * @param player Player entity data (position)
 * @param enemies Vector of enemy entities (positions and types)
 * @param diff Current difficulty level settings
 * @return true if save successful, false otherwise
 */
bool saveGame(int level, double gpa, const Entity& player, const vector<Entity>& enemies, const DifficultyLevel& diff);

/**
 * @brief Loads game state from saved file
 * @param level Output parameter for loaded level number
 * @param gpa Output parameter for loaded GPA value  
 * @param player Output parameter for loaded player data
 * @param enemies Output parameter for loaded enemy entities
 * @param diff Output parameter for loaded difficulty settings
 * @return true if load successful, false otherwise
 */
bool loadGame(int& level, double& gpa, Entity& player, vector<Entity>& enemies, DifficultyLevel& diff);

#endif
