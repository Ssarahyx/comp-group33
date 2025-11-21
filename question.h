#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

using namespace std;

struct set_difficulty {
    double initialGPA;
    double ta_penalty_k;
    double prof_penalty_k;
    double stu_penalty_k;
};

struct Qs {
    string questionText;
    string answer;
    double basePenalty;
};

string trim(const string& str);
void initQsRandom();
void load_All_Qs();
void load_TA_Qs(const string& filename);
void load_Prof_Qs(const string& filename);
void load_Stu_Qs(const string& filename);
double ask(char enemyType, const set_difficulty& difficulty);

#endif
