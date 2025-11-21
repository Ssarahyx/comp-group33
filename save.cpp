#include "save.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

DifficultyLevel easy() {
    DifficultyLevel diff;
    diff.name = "EASY";
    diff.initialGPA = 4.0;
    diff.ta_k = 0.8;
    diff.prof_k = 1.0;
    diff.stu_k = 0.5;
    return diff;
}

DifficultyLevel normal() {
    DifficultyLevel diff;
    diff.name = "NORMAL";
    diff.initialGPA = 3.5;
    diff.ta_k = 1.0;
    diff.prof_k = 1.5;
    diff.stu_k = 0.8;
    return diff;
}

DifficultyLevel hard() {
    DifficultyLevel diff;
    diff.name = "HARD";
    diff.initialGPA = 3.0;
    diff.ta_k = 1.2;
    diff.prof_k = 2.0;
    diff.stu_k = 1.0;
    return diff;
}

bool saveGame(int level, double gpa, const Entity& player,
              const vector<Entity>& enemies, const DifficultyLevel& diff) {
    
    string filename = "hku_gpa_escape_save.txt";
    ofstream file(filename);
    
    if (file.fail()) {
        cout << "Error: Cannot create save file: " << filename << endl;
        return false;
    }
    
    file << "LEVEL " << level << endl;
    file << "GPA " << gpa << endl;
    file << "DIFFICULTY " << diff.name << endl;
    
    file << "PLAYER " << player.x << " " << player.y << endl;
    
    file << "ENEMIES " << enemies.size() << endl;
    for (int i = 0; i < enemies.size(); i++) {
        file << enemies[i].type << " " << enemies[i].x << " " << enemies[i].y << endl;
    }
    
    file.close();
    cout << "Game saved successfully to " << filename << endl;
    return true;
}

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
            if (diffName == "EASY") {
                diff = easy();
            } else if (diffName == "NORMAL") {
                diff = normal();
            } else if (diffName == "HARD") {
                diff = hard();
            }else {
                diff = normal();
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
                    i--;
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