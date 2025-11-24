#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <functional>
#include "map.h"
#include "question.h"
#include "save.h"
#include "entity.h"

using namespace std;

/**
 * @brief Enumeration representing the different states of the game
 */
enum class GameState {
    MAIN_MENU,      ///< Displaying the main menu
    PLAYING,        ///< Actively playing a level
    LEVEL_COMPLETE, ///< Completed a level, ready for next
    GAME_OVER,      ///< Game ended due to failure
    VICTORY         ///< Game completed successfully
};

/**
 * @brief Main game controller class that manages the entire game flow
 * 
 * This class integrates all modules (map, entity, question, save systems)
 * and controls the game from start to finish through 3 levels with
 * difficulty-based progression and GPA management.
 */
class Game {
private:
    GameDifficultySettings currentDifficulty; ///< Current difficulty settings
    GameState currentState;                   ///< Current state of the game
    double currentGPA;                        ///< Player's current GPA value
    int currentLevel;                         ///< Current level (1-3)
    bool gameRunning;                         ///< Flag indicating if game is active
    
    GameConfig gameConfig;                    ///< Configuration for current game
    Entity player;                            ///< Player entity with position and status
    vector<Entity> enemies;                   ///< List of all enemy entities in current level
    
    // Core game flow methods
    
    /**
     * @brief Initializes the game with default settings
     */
    void initializeGame();
    
    /**
     * @brief Displays the main menu interface
     */
    void showMainMenu();
    
    /**
     * @brief Handles user input from the main menu
     */
    void handleMenuInput();
    
    /**
     * @brief Shows difficulty selection screen and processes choice
     */
    void selectDifficulty();
    
    /**
     * @brief Sets game difficulty based on user selection
     * @param difficultyChoice User's difficulty selection (1-3)
     */
    void setDifficulty(int difficultyChoice);
    
    /**
     * @brief Configures game parameters based on selected difficulty
     */
    void setupGameConfig();
    
    /**
     * @brief Loads and prepares the specified level
     * @param level Level number to load (1-3)
     */
    void loadLevel(int level);
    
    /**
     * @brief Initializes enemies by scanning the loaded map
     */
    void initializeEnemiesFromMap();
    
    /**
     * @brief Main game loop for active gameplay
     */
    void gameLoop();
    
    /**
     * @brief Processes player's turn including movement and actions
     */
    void playerTurn();
    
    /**
     * @brief Processes all enemy movements and AI behavior
     */
    void enemyTurn();
    
    /**
     * @brief Checks if player has encountered any enemies
     */
    void checkEncounters();
    
    /**
     * @brief Handles question/answer sequence when encountering enemies
     * @param enemyType Type of enemy encountered ('T', 'F', or 'S')
     */
    void handleQuestion(char enemyType);
    
    /**
     * @brief Updates player's GPA with specified change
     * @param change Amount to change GPA by (positive or negative)
     */
    void updateGPA(double change);
    
    /**
     * @brief Checks if game should end due to victory or failure
     */
    void checkGameState();
    
    /**
     * @brief Displays current game status including map and stats
     */
    void displayGameInfo();
    
    /**
     * @brief Handles game completion (both victory and failure)
     * @param victory True if player won, false if game over
     */
    void gameOver(bool victory);
    
    /**
     * @brief Saves current game state to file
     */
    void saveGameState();
    
    /**
     * @brief Loads game state from saved file
     * @return True if load successful, false otherwise
     */
    bool loadGameState();
    
    /**
     * @brief Adapter function to bridge entity system with map system
     * @param x X-coordinate (entity system)
     * @param y Y-coordinate (entity system)
     * @return True if position is walkable
     */
    bool isWalkableAdapter(int x, int y);

public:
    /**
     * @brief Constructs a new Game object with default initialization
     */
    Game();
    
    /**
     * @brief Main entry point that runs the complete game from start to finish
     */
    void run();
};

#endif
