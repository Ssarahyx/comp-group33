#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include "map.h"
#include "question.h"
#include "save.h"
using namespace std;

// Game state enumeration
enum class GameState {
    MAIN_MENU,
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER,
    VICTORY
};

class Game {
private:
    DifficultyLevel currentDifficulty;
    GameState currentState;
    double currentGPA;
    int currentLevel;
    bool gameRunning;
    
    // Player entity
    Entity player;
    
    // Enemy list
    vector<Entity> enemies;
    
    // Initialize game
    void initializeGame();
    
    // Display main menu
    void showMainMenu();
    
    // Handle menu input
    void handleMenuInput();
    
    // Select difficulty
    void selectDifficulty();
    
    // Set difficulty
    void setDifficulty(int difficultyChoice);
    
    // Load level
    void loadLevel(int level);
    
    // Initialize enemies
    void initializeEnemies();
    
    // Main game loop
    void gameLoop();
    
    // Player turn
    void playerTurn();
    
    // Enemy turn
    void enemyTurn();
    
    // Check encounters
    void checkEncounters();
    
    // Handle question
    void handleQuestion(char enemyType);
    
    // Update GPA
    void updateGPA(double change);
    
    // Check game state
    void checkGameState();
    
    // Display game info
    void displayGameInfo();
    
    // Game over handling
    void gameOver(bool victory);
    
    // Save game
    void saveGameState();
    
    // Load game
    bool loadGameState();

public:
    // Constructor
    Game();
    
    // Run game
    void run();
};

#endif
