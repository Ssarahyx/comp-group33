#ifndef SAVE_H
#define SAVE_H

#include <string>
#include <vector>

using namespace std;

// Unified difficulty level definitions
enum DifficultyLevelEnum {
    EASY = 1,
    NORMAL = 2, 
    HARD = 3
};

// Unified entity structure definition
struct Entity {
    int x;
    int y;
    char type;
    bool active;
    int id;
};

// Game difficulty settings
struct GameDifficultySettings {
    string name;
    double initialGPA;
    double ta_k;
    double prof_k;
    double stu_k;
};

// Function declarations

/**
 * @brief Creates easy difficulty settings
 * 
 * This function returns game parameters for easy difficulty level, including:
 * - Initial GPA of 4.0
 * - Lower enemy difficulty coefficients
 * - Reduced impact from various game entities
 * 
 * @return GameDifficultySettings Easy difficulty configuration object
 */
GameDifficultySettings easy();

/**
 * @brief Creates normal difficulty settings
 * 
 * This function returns game parameters for normal difficulty level, including:
 * - Initial GPA of 3.5
 * - Balanced enemy difficulty coefficients
 * - Standard impact from various game entities
 * 
 * @return GameDifficultySettings Normal difficulty configuration object
 */
GameDifficultySettings normal();

/**
 * @brief Creates hard difficulty settings
 * 
 * This function returns game parameters for hard difficulty level, including:
 * - Initial GPA of 3.0
 * - Higher enemy difficulty coefficients
 * - Increased impact from various game entities
 * 
 * @return GameDifficultySettings Hard difficulty configuration object
 */
GameDifficultySettings hard();

/**
 * @brief Saves the current game state to a file
 * 
 * This function serializes and saves the current game state including:
 * - Current level progress
 * - Player's GPA
 * - Player entity data (position, type, status, ID)
 * - All enemy entities data
 * - Current difficulty settings
 * - Map layout data (rows, columns, and tile contents)
 * 
 * @param level Current level number to save
 * @param gpa Current GPA value to save
 * @param player Player entity data to save
 * @param enemies Vector of enemy entities to save
 * @param diff Current difficulty settings to save
 * @return bool True if save operation succeeded, false otherwise
 */
bool saveGame(int level, double gpa, const Entity& player, const vector<Entity>& enemies, const GameDifficultySettings& diff);

/**
 * @brief Loads a previously saved game state from file
 * 
 * This function reads and deserializes a saved game state including:
 * - Previously saved level progress
 * - Player's GPA at time of save
 * - Player entity data (position, type, status, ID)
 * - All enemy entities data
 * - Difficulty settings used in saved game
 * - Map layout data from saved game
 * 
 * @param level Output parameter for loaded level number
 * @param gpa Output parameter for loaded GPA value
 * @param player Output parameter for loaded player entity data
 * @param enemies Output parameter for loaded enemy entities vector
 * @param diff Output parameter for loaded difficulty settings
 * @return bool True if load operation succeeded, false otherwise
 */
bool loadGame(int& level, double& gpa, Entity& player, vector<Entity>& enemies, GameDifficultySettings& diff);

#endif
