#include "game.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>  // 添加头文件
#include <cmath>   // 添加头文件

using namespace std;

/**
 * @brief Constructs a new Game object and initializes all systems
 * 
 * Sets up random number generation, initial game state, default difficulty,
 * and prepares the game for the main menu.
 */
Game::Game() {
    srand(time(0)); // Initialize random seed for game events - 保留这一处srand
    currentState = GameState::MAIN_MENU;
    gameRunning = true;
    currentLevel = 1;
    currentGPA = 0.0;
    gameConfig = {2, 0, 0, 0, 0}; // Default to NORMAL difficulty (level 2)
    currentDifficulty = normal(); // Set default difficulty settings
}

/**
 * @brief Adapter function to convert between entity and map coordinate systems
 * 
 * The entity system uses (x,y) where x is horizontal, y is vertical.
 * The map system uses (row,col) where row is vertical, col is horizontal.
 * This function translates between these coordinate systems.
 * 
 * @param x X-coordinate from entity system (horizontal position)
 * @param y Y-coordinate from entity system (vertical position)
 * @return True if the position is walkable, false if blocked by wall
 */
bool Game::isWalkableAdapter(int x, int y) {
    return position_walkable(y, x); // Convert (x,y) to (row,col) for map system
}

/**
 * @brief Main game execution loop that manages state transitions
 * 
 * Continuously runs the game, transitioning between different states
 * (menu, gameplay, level completion, game over, victory) until the
 * player chooses to exit the game.
 */
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
                    gameOver(true); // Player completed all 3 levels
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

/**
 * @brief Displays the main menu with game options
 * 
 * Presents the game title and available options to the player:
 * 1. Start New Game
 * 2. Load Game
 * 3. Exit Game
 */
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

/**
 * @brief Processes user input from the main menu
 * 
 * Reads player's menu selection and transitions to the appropriate
 * game state or exits the application.
 */
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

/**
 * @brief Displays difficulty selection screen and processes choice
 * 
 * Presents three difficulty options with their starting GPA values:
 * - Easy: 4.0 GPA
 * - Normal: 3.5 GPA  
 * - Hard: 3.0 GPA
 * 
 * After selection, initializes the game with chosen difficulty.
 */
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

/**
 * @brief Configures game settings based on selected difficulty
 * 
 * @param difficultyChoice Integer representing difficulty (1=Easy, 2=Normal, 3=Hard)
 */
void Game::setDifficulty(int difficultyChoice) {
    switch (difficultyChoice) {
        case 1:
            currentDifficulty = easy();
            gameConfig.level = 1; // EASY
            break;
        case 2:
            currentDifficulty = normal();
            gameConfig.level = 2; // NORMAL
            break;
        case 3:
            currentDifficulty = hard();
            gameConfig.level = 3; // HARD
            break;
        default:
            cout << "Invalid choice, using Normal difficulty" << endl;
            currentDifficulty = normal();
            gameConfig.level = 2; // NORMAL
            break;
    }
    currentGPA = currentDifficulty.initialGPA;
    setupGameConfig();
}

/**
 * @brief Configures enemy counts based on selected difficulty
 * 
 * Sets the number of each enemy type (TA, Professor, Student)
 * according to the chosen difficulty level to provide appropriate
 * challenge scaling.
 */
void Game::setupGameConfig() {
    switch (gameConfig.level) {
        case 1: // EASY - Fewer challenging enemies
            gameConfig.taCount = 2;
            gameConfig.professorCount = 1;
            gameConfig.studentCount = 3;
            break;
        case 2: // NORMAL - Balanced enemy distribution
            gameConfig.taCount = 3;
            gameConfig.professorCount = 2;
            gameConfig.studentCount = 4;
            break;
        case 3: // HARD - More challenging enemies
            gameConfig.taCount = 4;
            gameConfig.professorCount = 3;
            gameConfig.studentCount = 5;
            break;
    }
}

/**
 * @brief Initializes all game systems for a new game
 * 
 * Loads question databases, randomizes question order,
 * and starts the first level of the game.
 */
void Game::initializeGame() {
    // Initialize question system with all question files
    load_All_Qs();
    initQsRandom();
    
    currentLevel = 1;
    loadLevel(currentLevel);
    currentState = GameState::PLAYING;
}

/**
 * @brief Loads and prepares the specified level for gameplay
 * 
 * @param level The level number to load (1, 2, or 3)
 * 
 * Loads the appropriate map based on difficulty and level,
 * places the player at the starting position, and scans the
 * map to initialize all enemy entities.
 */
void Game::loadLevel(int level) {
    cout << "\n==================================" << endl;
    cout << "        Entering Level " << level << "         " << endl;
    cout << "==================================" << endl;
    
    // Load map with current difficulty and level
    load_map(gameConfig.level, level);
    
    // Initialize player at the map's starting position
    player = initPlayer(map_player_start_col, map_player_start_row);
    
    // Scan map and initialize all enemy entities
    initializeEnemiesFromMap();
    
    cout << "Level " << level << " loaded successfully!" << endl;
    cout << "Objective: Find the exit (E) and escape!" << endl;
}

/**
 * @brief Scans the loaded map to find and initialize all enemies
 * 
 * Iterates through every position in the map to locate enemy
 * characters ('T', 'F', 'S') and creates corresponding entity
 * objects with proper positions and attributes.
 */
void Game::initializeEnemiesFromMap() {
    enemies.clear();
    
    // Scan entire map for enemy characters
    for (int row = 0; row < map_rows; ++row) {
        for (int col = 0; col < map_cols; ++col) {
            char cell = get_map_char_at(row, col);
            if (cell == 'T' || cell == 'F' || cell == 'S') {
                Entity enemy;
                enemy.x = col;      // Entity system: x = column
                enemy.y = row;      // Entity system: y = row
                enemy.type = cell;
                enemy.active = true;
                enemy.id = enemies.size() + 1;
                enemies.push_back(enemy);
                
                // 关键：清掉地图上的敌人字符，防止"幽灵敌人"
                map_data[row][col] = '.';
            }
        }
    }
    
    cout << "This level has " << enemies.size() << " enemies" << endl;
}

/**
 * @brief Main gameplay loop for active level progression
 * 
 * Repeatedly cycles through displaying game info, processing
 * player turns, enemy turns, and checking game state until
 * the level is completed or the game ends.
 */

void Game::gameLoop() {
    while (currentState == GameState::PLAYING) {
        displayGameInfo();
        
        Entity* collidedEnemy = checkPlayerCollision(player, enemies);
        if (collidedEnemy != nullptr) {
            handleQuestion(collidedEnemy->type);
            if (currentGPA <= 0) {
                currentState = GameState::GAME_OVER;
                continue;
            }
        }

        bool savedThisTurn = playerTurn();
        
        if (savedThisTurn) {
    
            continue;
        }

            if (at_exit_position(player.y, player.x)) {
            cout << "\nCongratulations! You found the exit!" << endl;
            currentState = GameState::LEVEL_COMPLETE;
            continue;
        }

        collidedEnemy = checkPlayerCollision(player, enemies);
        if (collidedEnemy != nullptr) {
            handleQuestion(collidedEnemy->type);
            if (currentGPA <= 0) {
                currentState = GameState::GAME_OVER;
                continue;
            }
        }

        enemyTurn();

        collidedEnemy = checkPlayerCollision(player, enemies);
        if (collidedEnemy != nullptr) {
            handleQuestion(collidedEnemy->type);
            if (currentGPA <= 0) {
                currentState = GameState::GAME_OVER;
                continue;
            }
        }

        checkGameState();
    }
}bool Game::playerTurn() {
    cout << "\nYour turn - Enter movement direction (W/A/S/D) or P to save game: ";
    char input;
    cin >> input;
    input = toupper(input);

    if (input == 'P') {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        saveGameState();
        return true;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool moved = movePlayer(player, input,
        [this](int x, int y) { return this->isWalkableAdapter(x, y); },
        map_cols, map_rows);

    if (moved) {
        cout << "Movement successful! New position: (" << player.x << ", " << player.y << ")" << endl;
    } else {
        cout << "Cannot move in that direction! There is an obstacle or invalid direction." << endl;
    }

    return false;
}
/**
 * @brief Processes all enemy movements for the current turn
 * 
 * Applies enemy AI behaviors based on enemy type:
 * - TAs: 65% chance to track player, 35% random movement
 * - Professors: Always track player directly
 * - Students: Always move randomly
 * 
 * Validates movements against map boundaries and obstacles.
 */
void Game::enemyTurn() {
    cout << "\nEnemy turn..." << endl;
    
    // Process all enemy movements using entity system
    moveEnemies(enemies, player,
               [this](int x, int y) { return this->isWalkableAdapter(x, y); },
               map_cols, map_rows);
    
    cout << "Enemy movement completed" << endl;
}

/**
 * @brief Handles the question/answer sequence when encountering enemies
 * 
 * @param enemyType Type of enemy encountered ('T', 'F', or 'S')
 * 
 * Retrieves an appropriate question based on enemy type and difficulty,
 * evaluates the player's answer, and applies GPA penalties for incorrect
 * answers. Deactivates enemy if answer is correct.
 */
void Game::handleQuestion(char enemyType) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    // Prepare difficulty settings for question system - 修复：正确初始化set_difficulty
    set_difficulty qsDiff{}; // 先全部清0
    qsDiff.initialGPA = currentDifficulty.initialGPA;
    
    // Set penalty multipliers based on enemy type
    if (enemyType == 'T') {
        qsDiff.ta_penalty_k = currentDifficulty.ta_k;
    } else if (enemyType == 'F') {
        qsDiff.prof_penalty_k = currentDifficulty.prof_k;
    } else if (enemyType == 'S') {
        qsDiff.stu_penalty_k = currentDifficulty.stu_k;
    }
    
    // Present question and get result
    double penalty = ask(enemyType, qsDiff);
    
    if (penalty > 0) {
        // Apply GPA penalty for incorrect answer
        updateGPA(-penalty);
    } else {
        // Correct answer - deactivate the enemy
        Entity* collidedEnemy = checkPlayerCollision(player, enemies);
        if (collidedEnemy != nullptr) {
            deactivateEnemy(*collidedEnemy);
            cout << "Enemy deactivated! You can pass through." << endl;
        }
    }
}

/**
 * @brief Updates the player's GPA with the specified change
 * 
 * @param change Amount to change GPA by (positive or negative)
 * 
 * Applies the change to current GPA, ensures GPA doesn't go below zero,
 * and displays the result to the player.
 */
void Game::updateGPA(double change) {
    currentGPA += change;
    if (currentGPA < 0) currentGPA = 0;
    cout << "GPA " << (change > 0 ? "increased" : "decreased") << " by " << std::fabs(change) << endl; // 使用std::fabs
    cout << "Current GPA: " << currentGPA << endl;
}

/**
 * @brief Checks if game should end due to GPA depletion
 * 
 * Transitions to GAME_OVER state if player's GPA has reached zero,
 * ending the game due to academic failure.
 */
void Game::checkGameState() {
    if (currentGPA <= 0) {
        currentState = GameState::GAME_OVER;
    }
}

/**
 * @brief Displays current game status including map and statistics
 * 
 * Shows current level, difficulty, GPA, player position, and the
 * complete game map with all entities, walls, and the exit.
 */
void Game::displayGameInfo() {
    cout << "\n==================================" << endl;
    cout << "       Level " << currentLevel << " - Game Status       " << endl;
    cout << "==================================" << endl;
    
    cout << "Difficulty: " << currentDifficulty.name << " | GPA: " << currentGPA << endl;
    cout << "Player position: (" << player.x << ", " << player.y << ")" << endl;
    
    // Prepare active enemies for map display
    vector<Entity> activeEnemies;
    for (const auto& enemy : enemies) {
        if (enemy.active) {
            activeEnemies.push_back(enemy);
        }
    }
    
    // Display map with current positions
    print_map(player.y, player.x, activeEnemies);
    
    cout << "Symbols: P=Player, T=TA, F=Professor, S=Student, #=Wall, .=Empty, E=Exit" << endl;
}

void Game::saveGameState() {
    bool success = saveGame(currentLevel, currentGPA, player, enemies, currentDifficulty);
    if (success) {
        cout << "Game saved successfully!" << endl;
    } else {
        cout << "Game save failed! Please try again." << endl;
    }
}

/**
 * @brief Loads a previously saved game state from file
 * 
 * @return True if load was successful, false if file missing or corrupted
 * 
 * Restores all game data from save file including level progression,
 * GPA, entity positions, and difficulty settings, then resumes gameplay
 * from the saved state.
 */
bool Game::loadGameState() {
    int loadedLevel;
    double loadedGPA;
    Entity loadedPlayer;
    vector<Entity> loadedEnemies;
    GameDifficultySettings loadedDifficulty;
    
    bool success = loadGame(loadedLevel, loadedGPA, loadedPlayer, loadedEnemies, loadedDifficulty);
    
    if (success) {
        load_All_Qs();
        initQsRandom();
        
        // Restore all game state from loaded data
        currentLevel = loadedLevel;
        currentGPA = loadedGPA;
        player = loadedPlayer;
        enemies = loadedEnemies;
        currentDifficulty = loadedDifficulty;
        
        // Update game configuration based on loaded difficulty
        if (currentDifficulty.name == "EASY") {
            gameConfig.level = 1;
        } else if (currentDifficulty.name == "NORMAL") {
            gameConfig.level = 2;
        } else if (currentDifficulty.name == "HARD") {
            gameConfig.level = 3;
        }
        setupGameConfig();
        
        // load_map(gameConfig.level, currentLevel);
        
        cout << "Game loaded successfully!" << endl;
        return true;
    } else {
        cout << "Game load failed!" << endl;
        return false;
    }
}

/**
 * @brief Handles game completion (both victory and failure states)
 * 
 * @param victory True if player successfully completed the game,
 *                false if game ended due to failure
 * 
 * Displays appropriate ending message based on outcome, frees
 * map resources, and offers options to return to main menu or exit.
 */
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
    
    // Free dynamically allocated map memory
    free_map();
    
    // Offer post-game options
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
