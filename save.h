#ifndef SAVE_H
#define SAVE_H

#include <string>
#include <vector>

using namespace std;

struct Entity {
    int x;
    int y;
    char type;
};

struct DifficultyLevel {
    string name;
    double initialGPA;
    double ta_k;
    double prof_k;
    double stu_k;
};

DifficultyLevel easy();
DifficultyLevel normal();
DifficultyLevel hard();

bool saveGame(int level, double gpa, const Entity& player, 
              const vector<Entity>& enemies, const DifficultyLevel& diff);
bool loadGame(int& level, double& gpa, Entity& player,
              vector<Entity>& enemies, DifficultyLevel& diff);

#endif