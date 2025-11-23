#include "save.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

/**
 * @brief Creates EASY difficulty settings with reduced penalties
 * @return DifficultyLevel with 4.0 starting GPA and lower penalty multipliers
 */
DifficultyLevel easy() {
    DifficultyLevel diff;
    diff.name = "EASY";
    diff.initialGPA = 4.0;
    diff.ta_k = 0.8;     // 20% reduced TA penalties
    diff.prof_k = 1.0;   // Standard Professor penalties  
    diff.stu_k = 0.5;    // 50% reduced Student penalties
    return diff;
}

/**
 * @brief Creates NORMAL difficulty settings with standard penalties
 * @return DifficultyLevel with 3.5 starting GPA and standard penalty multipliers
 */
DifficultyLevel normal() {
    DifficultyLevel diff;
    diff.name = "NORMAL";
    diff.initialGPA = 3.5;
    diff.ta_k = 1.0;     // Standard TA penalties
    diff.prof_k = 1.5;   // 50% increased Professor penalties
    diff.stu_k = 0.8;    // 20% reduced Student penalties
    return diff;
}

/**
 * @brief Creates HARD difficulty settings with increased penalties
 * @return DifficultyLevel with 3.0 starting GPA and higher penalty multipliers
 */
DifficultyLevel hard() {
    DifficultyLevel diff;
    diff.name = "HARD";
    diff.initialGPA = 3.0;
    diff.ta_k = 1.2;     // 20% increased TA penalties
    diff.prof_k = 2.0;   // 100% increased Professor penalties
    diff.stu_k = 1.0;    // Standard Student penalties
    return diff;
}


/**
 * @brief Saves current game state to text file "hku_gpa_escape_save.txt"
 * File format includes level, GPA, difficulty, player position, and enemy data
 * @param level Current game level number
 * @param gpa Current player GPA value
 * @param player Player entity with position data
 * @param enemies Vector of enemy entities with positions and types
 * @param diff Current difficulty level settings
 * @return true if save successful, false if file creation fails
 */
bool saveGame(int level, double gpa, const Entity& player,
              const vector<Entity>& enemies, const DifficultyLevel& diff) {
    
    string filename = "hku_gpa_escape_save.txt";
    ofstream file(filename);
    
    if (file.fail()) {
        cout << "Error: Cannot create save file: " << filename << endl;
        return false;
    }

    // Write game state data
    file << "LEVEL " << level << endl;
    file << "GPA " << gpa << endl;
    file << "DIFFICULTY " << diff.name << endl;
    
    file << "PLAYER " << player.x << " " << player.y << endl;

    // Write enemy data
    file << "ENEMIES " << enemies.size() << endl;
    for (int i = 0; i < enemies.size(); i++) {
        file << enemies[i].type << " " << enemies[i].x << " " << enemies[i].y << endl;
    }
    
    file.close();
    cout << "Game saved successfully to " << filename << endl;
    return true;
}

/**
 * @brief Loads game state from "hku_gpa_escape_save.txt" file
 * Parses saved data and restores game state to parameters
 * @param level Output parameter for loaded level number
 * @param gpa Output parameter for loaded GPA value
 * @param player Output parameter for loaded player entity
 * @param enemies Output parameter for loaded enemy entities vector
 * @param diff Output parameter for loaded difficulty settings
 * @return true if load successful, false if file missing or corrupted
 */
bool loadGame(int& level, double& gpa, Entity& player,
    vector<Entity>& enemies, DifficultyLevel& diff) {

string filename = "hku_gpa_escape_save.txt";
ifstream file(filename);

if (!file.is_open()) {
    cout << "Error: Cannot open save file: " << filename << endl;
    return false;
}

string line;
enemies.clear();
bool success = true;

// Parse each line of the save file
while (getline(file, line) && success) {
    if (line.empty()) continue;

    istringstream iss(line);
    string token;
    iss >> token;

    if (token == "LEVEL") {
        if (!(iss >> level)) {
            cout << "Error reading level" << endl;
            success = false;
        }
    } else if (token == "GPA") {
            if (!(iss >> gpa)) {
                cout << "Error reading GPA" << endl;
                success = false;
            }
    } else if (token == "DIFFICULTY") {
        string diffName;
        if (!(iss >> diffName)) {
            cout << "Error reading difficulty" << endl;
            success = false;
        } else {
            // Map difficulty name to settings
            if (diffName == "EASY") {
                diff = easy();
            } else if (diffName == "NORMAL") {
                diff = normal();
            } else if (diffName == "HARD") {
                diff = hard();
            }else {
                diff = normal(); // Default fallback
            }
        }
    } else if (token == "PLAYER") {
        if (!(iss >> player.x >> player.y)) {
            cout << "Error reading player position" << endl;
            success = false;
        } else {
            player.type = 'P';
        }
    } else if (token == "ENEMIES") {
        int enemyCount;
        if (!(iss >> enemyCount)) {
            cout << "Error reading enemy count" << endl;
            success = false;
        } else {
            // Load each enemy entity
            for (int i = 0; i < enemyCount && success; i++) {
                if (!getline(file, line)) {
                    cout << "Error: Expected " << enemyCount << " enemies but got only " << i << endl;
                    success = false;
                } else if (!line.empty()) {
                    istringstream enemyIss(line);
                    Entity enemy;
                    if (enemyIss >> enemy.type >> enemy.x >> enemy.y) {
                        enemies.push_back(enemy);
                    } else {
                        cout << "Error reading enemy data: '" << line << "'" << endl;
                        success = false;
                    }
                } else {
                    i--; // Skip empty lines
                }
            }
        }
    }
}

file.close();

if (success) {
cout << "Game loaded successfully from " << filename << endl;
return true;
} else {
cout << "Failed to load game" << endl;
return false;
}
}
