#include "save.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>  // Added for numeric_limits

using namespace std;

// External map data variables (need to be declared in your main program)
extern int map_rows;
extern int map_cols; 
extern char** map_data;
extern void free_map();  // Function to free existing map memory

GameDifficultySettings easy() {
    GameDifficultySettings diff;
    diff.name = "EASY";
    diff.initialGPA = 4.0;   // Highest starting GPA
    diff.ta_k = 0.8;         // Reduced TA penalty multiplier
    diff.prof_k = 1.0;       // Standard professor penalty
    diff.stu_k = 0.5;        // Greatly reduced student penalty
    return diff;
}

GameDifficultySettings normal() {
    GameDifficultySettings diff;
    diff.name = "NORMAL";
    diff.initialGPA = 3.5;
    diff.ta_k = 1.0;
    diff.prof_k = 1.5;
    diff.stu_k = 0.8;
    return diff;
}

GameDifficultySettings hard() {
    GameDifficultySettings diff;
    diff.name = "HARD";
    diff.initialGPA = 3.0;
    diff.ta_k = 1.2;
    diff.prof_k = 2.0;
    diff.stu_k = 1.0;
    return diff;
}

bool saveGame(int level, double gpa, const Entity& player,
              const vector<Entity>& enemies, const GameDifficultySettings& diff) {
    
    string filename = "hku_gpa_escape_save.txt";
    ofstream file(filename);
    
    if (file.fail()) {
        cout << "Error: Cannot create save file: " << filename << endl;
        return false;
    }
    
    // Save game metadata
    file << "LEVEL " << level << endl;
    file << "GPA " << gpa << endl;
    file << "DIFFICULTY " << diff.name << endl;
    file << "PLAYER " << player.x << " " << player.y << " " << player.type << " " << player.active << " " << player.id << endl;
    
    // Save enemy data with count
    file << "ENEMIES " << enemies.size() << endl;
    for (const auto& enemy : enemies) {
        file << enemy.type << " " << enemy.x << " " << enemy.y << " " << enemy.active << " " << enemy.id << endl;
    }
    
    // Save map data to preserve layout
    file << "MAP " << map_rows << " " << map_cols << endl;
    for (int r = 0; r < map_rows; ++r) {
        for (int c = 0; c < map_cols; ++c) {
            file << map_data[r][c]; // Write each map cell
        }
        file << '\n'; // Newline after each row
    }
    
    file.close();
    cout << "Game saved successfully to " << filename << endl;
    return true;
}

bool loadGame(int& level, double& gpa, Entity& player,
    vector<Entity>& enemies, GameDifficultySettings& diff) {
    
    string filename = "hku_gpa_escape_save.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot open save file: " << filename << endl;
        return false;
    }

    string line;
    enemies.clear(); // Clear existing enemies before loading
    bool success = true;
    bool mapLoaded = false; // Flag to ensure map data was loaded

    // Parse each line of the save file
    while (getline(file, line) && success) {
        if (line.empty()) continue; // Skip empty lines

        istringstream iss(line);
        string token;
        iss >> token; // Get the first token to identify data type

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
                // Set difficulty based on saved name
                if (diffName == "EASY") {
                    diff = easy();
                } else if (diffName == "NORMAL") {
                    diff = normal();
                } else if (diffName == "HARD") {
                    diff = hard();
                } else {
                    diff = normal(); // Default to normal if unknown
                }
            }
        } else if (token == "PLAYER") {
            if (!(iss >> player.x >> player.y >> player.type >> player.active >> player.id)) {
                cout << "Error reading player data" << endl;
                success = false;
            }
        } else if (token == "ENEMIES") {
            int enemyCount;
            if (!(iss >> enemyCount)) {
                cout << "Error reading enemy count" << endl;
                success = false;
            } else {
                // Read each enemy entry
                for (int i = 0; i < enemyCount && success; i++) {
                    if (!getline(file, line)) {
                        cout << "Error: Expected " << enemyCount << " enemies but got only " << i << endl;
                        success = false;
                    } else if (!line.empty()) {
                        istringstream enemyIss(line);
                        Entity enemy;
                        if (enemyIss >> enemy.type >> enemy.x >> enemy.y >> enemy.active >> enemy.id) {
                            enemies.push_back(enemy);
                        } else {
                            cout << "Error reading enemy data: '" << line << "'" << endl;
                            success = false;
                        }
                    } else {
                        i--; // Decrement counter if line was empty
                    }
                }
            }
        } else if (token == "MAP") {
            int rows, cols;
            if (!(iss >> rows >> cols)) {
                cout << "Error reading map dimensions" << endl;
                success = false;
            } else {
                // Clear the old map before loading new one
                free_map();
                
                // Set new dimensions
                map_rows = rows;
                map_cols = cols;
                
                // Allocate new map memory
                map_data = new char*[map_rows];
                for (int r = 0; r < map_rows; ++r) {
                    map_data[r] = new char[map_cols];
                }
                
                // Read map data
                
                for (int r = 0; r < map_rows && success; ++r) {
                    if (!getline(file, line)) {
                        cout << "Error reading map row " << r << endl;
                        success = false;
                    } else {
                        for (int c = 0; c < map_cols; ++c) {
                            if (c < (int)line.size()) {
                                map_data[r][c] = line[c];
                            } else {
                                map_data[r][c] = '.'; // Default to empty space
                            }
                        }
                    }
                }
                mapLoaded = true; // Mark map as successfully loaded
            }
        }
    }

    file.close();

    // Verify successful loading of all required components
    if (success && mapLoaded) {
        cout << "Game loaded successfully from " << filename << endl;
        return true;
    } else {
        if (!mapLoaded) {
            cout << "Error: Map data not found in save file" << endl;
        }
        cout << "Failed to load game" << endl;
        return false;
    }
}
