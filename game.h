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

// 游戏状态枚举
enum class GameState {
    MAIN_MENU,
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER,
    VICTORY
};

class Game {
private:
    GameDifficultySettings currentDifficulty;
    GameState currentState;
    double currentGPA;
    int currentLevel;
    bool gameRunning;
    
    GameConfig gameConfig;
    Entity player;
    vector<Entity> enemies;
    
    // 核心函数
    void initializeGame();
    void showMainMenu();
    void handleMenuInput();
    void selectDifficulty();
    void setDifficulty(int difficultyChoice);
    void setupGameConfig();
    void loadLevel(int level);
    void initializeEnemiesFromMap();
    void gameLoop();
    void playerTurn();
    void enemyTurn();
    void checkEncounters();
    void handleQuestion(char enemyType);
    void updateGPA(double change);
    void checkGameState();
    void displayGameInfo();
    void gameOver(bool victory);
    void saveGameState();
    bool loadGameState();
    bool isWalkableAdapter(int x, int y);

public:
    Game();
    void run();
};

#endif
