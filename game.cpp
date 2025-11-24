#include "game.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

Game::Game() {
    srand(time(0));
    currentState = GameState::MAIN_MENU;
    gameRunning = true;
    currentLevel = 1;
    currentGPA = 0.0;
    gameConfig = {2, 0, 0, 0}; // 默认Normal难度
    currentDifficulty = normal();
}

bool Game::isWalkableAdapter(int x, int y) {
    return position_walkable(y, x);
}

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
                    gameOver(true);
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

void Game::setDifficulty(int difficultyChoice) {
    switch (difficultyChoice) {
        case 1:
            currentDifficulty = easy();
            gameConfig.level = 1;
            break;
        case 2:
            currentDifficulty = normal();
            gameConfig.level = 2;
            break;
        case 3:
            currentDifficulty = hard();
            gameConfig.level = 3;
            break;
        default:
            cout << "Invalid choice, using Normal difficulty" << endl;
            currentDifficulty = normal();
            gameConfig.level = 2;
            break;
    }
    currentGPA = currentDifficulty.initialGPA;
    setupGameConfig();
}

void Game::setupGameConfig() {
    switch (gameConfig.level) {
        case 1: // EASY
            gameConfig.taCount = 2;
            gameConfig.professorCount = 1;
            gameConfig.studentCount = 3;
            break;
        case 2: // NORMAL
            gameConfig.taCount = 3;
            gameConfig.professorCount = 2;
            gameConfig.studentCount = 4;
            break;
        case 3: // HARD
            gameConfig.taCount = 4;
            gameConfig.professorCount = 3;
            gameConfig.studentCount = 5;
            break;
    }
}

void Game::initializeGame() {
    load_All_Qs();
    initQsRandom();
    currentLevel = 1;
    loadLevel(currentLevel);
    currentState = GameState::PLAYING;
}

void Game::loadLevel(int level) {
    cout << "\n==================================" << endl;
    cout << "        Entering Level " << level << "         " << endl;
    cout << "==================================" << endl;
    
    load_map(gameConfig.level, level);
    player = initPlayer(map_player_start_col, map_player_start_row);
    initializeEnemiesFromMap();
    
    cout << "Level " << level << " loaded successfully!" << endl;
    cout << "Objective: Find the exit (E) and escape!" << endl;
}

void Game::initializeEnemiesFromMap() {
    enemies.clear();
    
    for (int row = 0; row < map_rows; ++row) {
        for (int col = 0; col < map_cols; ++col) {
            char cell = get_map_char_at(row, col);
            if (cell == 'T' || cell == 'F' || cell == 'S') {
                Entity enemy;
                enemy.x = col;
                enemy.y = row;
                enemy.type = cell;
                enemy.active = true;
                enemy.id = enemies.size() + 1;
                enemies.push_back(enemy);
            }
        }
    }
    
    cout << "This level has " << enemies.size() << " enemies" << endl;
}

void Game::gameLoop() {
    while (currentState == GameState::PLAYING) {
        displayGameInfo();
        playerTurn();
        
        if (at_exit_position(player.y, player.x)) {
            cout << "\nCongratulations! You found the exit!" << endl;
            currentState = GameState::LEVEL_COMPLETE;
            continue;
        }
        
        enemyTurn();
        checkEncounters();
        checkGameState();
    }
}

void Game::playerTurn() {
    cout << "\nYour turn - Enter movement direction (W/A/S/D) or S to save game: ";
    char input;
    cin >> input;
    input = toupper(input);
    
    if (input == 'S') {
        saveGameState();
        return;
    }
    
    bool moved = movePlayer(player, input, 
                           [this](int x, int y) { return this->isWalkableAdapter(x, y); },
                           map_cols, map_rows);
    
    if (moved) {
        cout << "Movement successful! New position: (" << player.x << ", " << player.y << ")" << endl;
    } else {
        cout << "Cannot move in that direction! There is an obstacle or invalid direction." << endl;
    }
}

void Game::enemyTurn() {
    cout << "\nEnemy turn..." << endl;
    
    moveEnemies(enemies, player,
               [this](int x, int y) { return this->isWalkableAdapter(x, y); },
               map_cols, map_rows);
    
    cout << "Enemy movement completed" << endl;
}

void Game::checkEncounters() {
    Entity* collidedEnemy = checkPlayerCollision(player, enemies);
    if (collidedEnemy != nullptr) {
        handleQuestion(collidedEnemy->type);
    }
}

void Game::handleQuestion(char enemyType) {
    set_difficulty qsDiff;
    qsDiff.initialGPA = currentDifficulty.initialGPA;
    
    if (enemyType == 'T') {
        qsDiff.ta_penalty_k = currentDifficulty.ta_k;
    } else if (enemyType == 'F') {
        qsDiff.prof_penalty_k = currentDifficulty.prof_k;
    } else if (enemyType == 'S') {
        qsDiff.stu_penalty_k = currentDifficulty.stu_k;
    }
    
    double penalty = ask(enemyType, qsDiff);
    
    if (penalty > 0) {
        updateGPA(-penalty);
    } else {
        Entity* collidedEnemy = checkPlayerCollision(player, enemies);
        if (collidedEnemy != nullptr) {
            deactivateEnemy(*collidedEnemy);
            cout << "Enemy deactivated! You can pass through." << endl;
        }
    }
}

void Game::updateGPA(double change) {
    currentGPA += change;
    if (currentGPA < 0) currentGPA = 0;
    cout << "GPA " << (change > 0 ? "increased" : "decreased") << " by " << abs(change) << endl;
    cout << "Current GPA: " << currentGPA << endl;
}

void Game::checkGameState() {
    if (currentGPA <= 0) {
        currentState = GameState::GAME_OVER;
    }
}

void Game::displayGameInfo() {
    cout << "\n==================================" << endl;
    cout << "       Level " << currentLevel << " - Game Status       " << endl;
    cout << "==================================" << endl;
    
    cout << "Difficulty: " << currentDifficulty.name << " | GPA: " << currentGPA << endl;
    cout << "Player position: (" << player.x << ", " << player.y << ")" << endl;
    
    vector<Entity> activeEnemies;
    for (const auto& enemy : enemies) {
        if (enemy.active) {
            activeEnemies.push_back(enemy);
        }
    }
    
    print_map(player.y, player.x, activeEnemies);
    cout << "Symbols: P=Player, T=TA, F=Professor, S=Student, #=Wall, .=Empty, E=Exit" << endl;
}

void Game::saveGameState() {
    bool success = saveGame(currentLevel, currentGPA, player, enemies, currentDifficulty);
    if (success) {
        cout << "Game saved successfully!" << endl;
    } else {
        cout << "Game save failed!" << endl;
    }
}

bool Game::loadGameState() {
    int loadedLevel;
    double loadedGPA;
    Entity loadedPlayer;
    vector<Entity> loadedEnemies;
    GameDifficultySettings loadedDifficulty;
    
    bool success = loadGame(loadedLevel, loadedGPA, loadedPlayer, loadedEnemies, loadedDifficulty);
    
    if (success) {
        currentLevel = loadedLevel;
        currentGPA = loadedGPA;
        player = loadedPlayer;
        enemies = loadedEnemies;
        currentDifficulty = loadedDifficulty;
        
        if (currentDifficulty.name == "EASY") {
            gameConfig.level = 1;
        } else if (currentDifficulty.name == "NORMAL") {
            gameConfig.level = 2;
        } else if (currentDifficulty.name == "HARD") {
            gameConfig.level = 3;
        }
        setupGameConfig();
        load_map(gameConfig.level, currentLevel);
        
        cout << "Game loaded successfully!" << endl;
        return true;
    } else {
        cout << "Game load failed!" << endl;
        return false;
    }
}

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
