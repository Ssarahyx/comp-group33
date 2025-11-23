#include "game.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

// Constructor
Game::Game() {
    srand(time(0)); // Random seed
    currentState = GameState::MAIN_MENU;
    gameRunning = true;
    currentLevel = 1;
    currentGPA = 0.0;
    
    // Initialize player
    player.x = 0;
    player.y = 0;
    player.type = 'P';
    
    // Set default difficulty
    currentDifficulty = normal();
}

// Run game
void Game::run() {
    while (gameRunning) {
        switch (currentState) {
            case GameState::MAIN_MENU:
                showMainMenu();
                handleMenuInput();
                break;
            case GameState::PLAYING:
                gameLoop();
                break;
            case GameState::LEVEL_COMPLETE:
                if (currentLevel < 3) {
                    currentLevel++;
                    loadLevel(currentLevel);
                    currentState = GameState::PLAYING;
                } else {
                    gameOver(true); // Win game
                }
                break;
            case GameState::GAME_OVER:
                gameOver(false);
                break;
            case GameState::VICTORY:
                gameOver(true);
                break;
        }
    }
}

// Display main menu
void Game::showMainMenu() {
    cout << "\n==================================" << endl;
    cout << "        HKU GPA Escape           " << endl;
    cout << "==================================" << endl;
    cout << "  Escape Academic Zombies!       " << endl;
    cout << "==================================" << endl;
    cout << "1. Start New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Exit Game" << endl;
    cout << "Enter choice (1-3): ";
}

// Handle menu input
void Game::handleMenuInput() {
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            selectDifficulty();
            break;
        case 2:
            if (loadGameState()) {
                currentState = GameState::PLAYING;
            }
            break;
        case 3:
            gameRunning = false;
            break;
        default:
            cout << "Invalid choice, please try again!" << endl;
            break;
    }
}

// Select difficulty
void Game::selectDifficulty() {
    cout << "\n==================================" << endl;
    cout << "        Select Difficulty        " << endl;
    cout << "==================================" << endl;
    cout << "1. Easy   (Initial GPA: 4.0)" << endl;
    cout << "2. Normal (Initial GPA: 3.5)" << endl;
    cout << "3. Hard   (Initial GPA: 3.0)" << endl;
    cout << "Enter difficulty (1-3): ";
    
    int choice;
    cin >> choice;
    
    setDifficulty(choice);
    cout << "\nDifficulty set! Starting game..." << endl;
    initializeGame();
}

// Set difficulty
void Game::setDifficulty(int difficultyChoice) {
    switch (difficultyChoice) {
        case 1:
            currentDifficulty = easy();
            break;
        case 2:
            currentDifficulty = normal();
            break;
        case 3:
            currentDifficulty = hard();
            break;
        default:
            cout << "Invalid choice, using Normal difficulty" << endl;
            currentDifficulty = normal();
            break;
    }
    currentGPA = currentDifficulty.initialGPA;
}

// Initialize game
void Game::initializeGame() {
    // Initialize question system
    load_All_Qs();
    initQsRandom();
    
    currentLevel = 1;
    loadLevel(currentLevel);
    currentState = GameState::PLAYING;
}

// Load level
void Game::loadLevel(int level) {
    cout << "\n==================================" << endl;
    cout << "        Entering Level " << level << "         " << endl;
    cout << "==================================" << endl;
    
    // Call B module's map loading function
    int difficultyValue = 1; // Default Normal
    if (currentDifficulty.name == "EASY") difficultyValue = 1;
    else if (currentDifficulty.name == "NORMAL") difficultyValue = 2;
    else if (currentDifficulty.name == "HARD") difficultyValue = 3;
    
    load_map(difficultyValue, level);
    
    // Set player starting position
    player.x = map_player_start_row;
    player.y = map_player_start_col;
    
    // Initialize enemies
    initializeEnemies();
    
    cout << "Level " << level << " loaded successfully!" << endl;
    cout << "Objective: Find the exit (E) and escape!" << endl;
}

// Initialize enemies
void Game::initializeEnemies() {
    enemies.clear();
    
    // Scan map to find all enemies
    for (int row = 0; row < map_rows; ++row) {
        for (int col = 0; col < map_cols; ++col) {
            char cell = get_map_char_at(row, col);
            if (cell == 'T' || cell == 'F' || cell == 'S') {
                Entity enemy;
                enemy.x = row;
                enemy.y = col;
                enemy.type = cell;
                enemies.push_back(enemy);
            }
        }
    }
    
    cout << "This level has " << enemies.size() << " enemies" << endl;
}

// Main game loop
void Game::gameLoop() {
    while (currentState == GameState::PLAYING) {
        displayGameInfo();
        playerTurn();
        
        // Check if reached exit
        if (at_exit_position(player.x, player.y)) {
            cout << "\nCongratulations! You found the exit!" << endl;
            currentState = GameState::LEVEL_COMPLETE;
            continue;
        }
        
        enemyTurn();
        checkEncounters();
        checkGameState();
    }
}

// Player turn
void Game::playerTurn() {
    cout << "\nYour turn - Enter movement direction (W/A/S/D) or S to save game: ";
    char input;
    cin >> input;
    
    // Convert to uppercase
    input = toupper(input);
    
    // Check if saving game
    if (input == 'S') {
        saveGameState();
        return;
    }
    
    int newX = player.x;
    int newY = player.y;
    
    switch (input) {
        case 'W': newX--; break;
        case 'S': newX++; break;
        case 'A': newY--; break;
        case 'D': newY++; break;
        default:
            cout << "Invalid direction! Use W/A/S/D or S to save game" << endl;
            return;
    }
    
    // Check if movement is possible
    if (position_walkable(newX, newY)) {
        player.x = newX;
        player.y = newY;
        cout << "Movement successful! New position: (" << player.x << ", " << player.y << ")" << endl;
    } else {
        cout << "Cannot move in that direction! There is an obstacle." << endl;
    }
}

// Enemy turn
void Game::enemyTurn() {
    cout << "\nEnemy turn..." << endl;
    
    // Simple enemy movement logic
    for (auto& enemy : enemies) {
        // Simple movement: 50% chance to move toward player
        if (rand() % 2 == 0) {
            int deltaX = 0, deltaY = 0;
            
            if (enemy.x < player.x) deltaX = 1;
            else if (enemy.x > player.x) deltaX = -1;
            
            if (enemy.y < player.y) deltaY = 1;
            else if (enemy.y > player.y) deltaY = -1;
            
            // Randomly choose to move in X or Y direction
            if (rand() % 2 == 0 && deltaX != 0) {
                int newX = enemy.x + deltaX;
                if (position_walkable(newX, enemy.y)) {
                    enemy.x = newX;
                }
            } else if (deltaY != 0) {
                int newY = enemy.y + deltaY;
                if (position_walkable(enemy.x, newY)) {
                    enemy.y = newY;
                }
            }
        }
    }
    
    cout << "Enemy movement completed" << endl;
}

// Check encounters
void Game::checkEncounters() {
    // Check if player overlaps with any enemy
    for (const auto& enemy : enemies) {
        if (enemy.x == player.x && enemy.y == player.y) {
            handleQuestion(enemy.type);
            break; // Only handle one encounter at a time
        }
    }
}

// Handle question
void Game::handleQuestion(char enemyType) {
    // Create set_difficulty structure for question system
    set_difficulty qsDiff;
    qsDiff.initialGPA = currentDifficulty.initialGPA;
    
    // Set penalty coefficients
    if (enemyType == 'T') {
        qsDiff.ta_penalty_k = currentDifficulty.ta_k;
    } else if (enemyType == 'F') {
        qsDiff.prof_penalty_k = currentDifficulty.prof_k;
    } else if (enemyType == 'S') {
        qsDiff.stu_penalty_k = currentDifficulty.stu_k;
    }
    
    // Call group member 2's question system
    double penalty = ask(enemyType, qsDiff);
    
    if (penalty > 0) {
        updateGPA(-penalty);
    }
}

// Update GPA
void Game::updateGPA(double change) {
    currentGPA += change;
    if (currentGPA < 0) currentGPA = 0;
    
    cout << "GPA " << (change > 0 ? "increased" : "decreased") << " by " << abs(change) << endl;
    cout << "Current GPA: " << currentGPA << endl;
}

// Check game state
void Game::checkGameState() {
    if (currentGPA <= 0) {
        currentState = GameState::GAME_OVER;
    }
}

// Display game info
void Game::displayGameInfo() {
    cout << "\n==================================" << endl;
    cout << "       Level " << currentLevel << " - Game Status       " << endl;
    cout << "==================================" << endl;
    
    cout << "Difficulty: " << currentDifficulty.name << " | GPA: " << currentGPA << endl;
    cout << "Player position: (" << player.x << ", " << player.y << ")" << endl;
    
    // Call B module to display map
    print_map(player.x, player.y, enemies);
    
    cout << "Symbols: P=Player, T=TA, F=Professor, S=Student, #=Wall, .=Empty, E=Exit" << endl;
}

// Save game
void Game::saveGameState() {
    bool success = saveGame(currentLevel, currentGPA, player, enemies, currentDifficulty);
    if (success) {
        cout << "Game saved successfully!" << endl;
    } else {
        cout << "Game save failed!" << endl;
    }
}

// Load game
bool Game::loadGameState() {
    int loadedLevel;
    double loadedGPA;
    Entity loadedPlayer;
    vector<Entity> loadedEnemies;
    DifficultyLevel loadedDifficulty;
    
    bool success = loadGame(loadedLevel, loadedGPA, loadedPlayer, loadedEnemies, loadedDifficulty);
    
    if (success) {
        currentLevel = loadedLevel;
        currentGPA = loadedGPA;
        player = loadedPlayer;
        enemies = loadedEnemies;
        currentDifficulty = loadedDifficulty;
        
        // Reload map
        int difficultyValue = 1;
        if (currentDifficulty.name == "EASY") difficultyValue = 1;
        else if (currentDifficulty.name == "NORMAL") difficultyValue = 2;
        else if (currentDifficulty.name == "HARD") difficultyValue = 3;
        
        load_map(difficultyValue, currentLevel);
        
        cout << "Game loaded successfully!" << endl;
        return true;
    } else {
        cout << "Game load failed!" << endl;
        return false;
    }
}

// Game over handling
void Game::gameOver(bool victory) {
    cout << "\n==================================" << endl;
    if (victory) {
        cout << "          VICTORY!              " << endl;
        cout << "You successfully escaped the building!" << endl;
        cout << "Final GPA: " << currentGPA << endl;
    } else {
        cout << "          GAME OVER             " << endl;
        cout << "GPA dropped to zero, academic failure..." << endl;
    }
    cout << "==================================" << endl;
    
    // Free map memory
    free_map();
    
    cout << "\n1. Return to Main Menu" << endl;
    cout << "2. Exit Game" << endl;
    cout << "Enter choice (1-2): ";
    
    int choice;
    cin >> choice;
    
    if (choice == 1) {
        currentState = GameState::MAIN_MENU;
    } else {
        gameRunning = false;
    }
}
