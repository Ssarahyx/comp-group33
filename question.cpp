#include "question.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <algorithm>
using namespace std;

vector<Qs> taQs;
vector<Qs> profQs;
vector<Qs> stuQs;

string trim(const string& str) {
    if (str.empty()) return "";
    int start = 0;
    int end = str.length() - 1;
    
    while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')) {
        start++;
    }
    
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r')) {
        end--;
    }
    
    if (start > end) return "";
    return str.substr(start, end - start + 1);
}


bool randomCompare(const Qs& a, const Qs& b) {
    return rand() % 2 == 0;
}

void initQsRandom() {
    srand(time(0));
    
    if (!taQs.empty()) {
        sort(taQs.begin(), taQs.end(), randomCompare);
    }
    if (!profQs.empty()) {
        sort(profQs.begin(), profQs.end(), randomCompare);
    }
    if (!stuQs.empty()) {
        sort(stuQs.begin(), stuQs.end(), randomCompare);
    }
    
    cout << "Random system initialized." << endl;
}

void load_All_Qs() {
    load_TA_Qs("questions_ta.txt");
    load_Prof_Qs("questions_prof.txt");
    load_Stu_Qs("questions_student.txt");
    cout << "All questions loaded successfully!" << endl;
}

void load_TA_Qs(const string& filename) {
    taQs = vector<Qs>();
    
    ifstream file(filename);
    if (file.fail()) {
        cout << "Error: Cannot open TA questions file: " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string question, answer;
        double penalty;
        
        if (getline(iss, question, '|') && getline(iss, answer, '|') && (iss >> penalty)) {
            Qs q;
            q.questionText = trim(question);
            q.answer = trim(answer);
            q.basePenalty = penalty;
            taQs.push_back(q);
        }
    }
    file.close();
    
    cout << "Loaded " << taQs.size() << " TA questions." << endl;
}

void load_Prof_Qs(const string& filename) {
    profQs = vector<Qs>();
    
    ifstream file(filename);
    if (file.fail()) {
        cout << "Error: Cannot open Professor questions file: " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string question, answer;
        double penalty;
        
        if (getline(iss, question, '|') && getline(iss, answer, '|') && (iss >> penalty)) { 
            Qs q;
            q.questionText = trim(question);
            q.answer = trim(answer);
            q.basePenalty = penalty;
            profQs.push_back(q);
        }
    }
    file.close();
    
    cout << "Loaded " << profQs.size() << " Professor questions." << endl;
}

void load_Stu_Qs(const string& filename) {
    stuQs = vector<Qs>();
    
    ifstream file(filename);
    if (file.fail()) {
        cout << "Error: Cannot open Student questions file: " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        
        istringstream iss(line);
        string question, answer;
        double penalty;
        
        if (getline(iss, question, '|') && getline(iss, answer, '|') && (iss >> penalty)) {
            Qs q;
            q.questionText = trim(question);
            q.answer = trim(answer);
            q.basePenalty = penalty;
            stuQs.push_back(q);
        }
    }
    file.close();
    
    cout << "Loaded " << stuQs.size() << " Student questions." << endl;
}

double ask(char enemyType, const set_difficulty& difficulty) {
    string enemyName;
    Qs selectedQuestion;
    double basePenalty = 0.0;
    bool foundQuestion = false;
    
    if (enemyType == 'T') {
        enemyName = "TA";
        if (taQs.empty()) {
            cout << enemyName << ": No questions available. You're lucky this time!" << endl;
            return 0.0;
        }
        int rd = rand() % taQs.size();
        selectedQuestion = taQs[rd];
        basePenalty = selectedQuestion.basePenalty;
        foundQuestion = true;
        
    } else if (enemyType == 'F') {
        enemyName = "Professor";
        if (profQs.empty()) {
            cout << enemyName << ": No questions available. You're lucky this time!" << endl;
            return 0.0;
        }
        int rd = rand() % profQs.size();
        selectedQuestion = profQs[rd];
        basePenalty = selectedQuestion.basePenalty;
        foundQuestion = true;
        
    } else if (enemyType == 'S') {
        enemyName = "Student";
        if (stuQs.empty()) {
            cout << enemyName << ": No questions available. You're lucky this time!" << endl;
            return 0.0;
        }
        int rd = rand() % stuQs.size();
        selectedQuestion = stuQs[rd];
        basePenalty = selectedQuestion.basePenalty;
        foundQuestion = true;
        
    } else {
        cout << "Error: Unknown enemy type: " << enemyType << endl;
        return 0.0;
    }
    
    if (!foundQuestion) {
        return 0.0;
    }
    
    cout << "\n=== " << enemyName << " Encounter! ===" << endl;
    cout << selectedQuestion.questionText << endl;
    cout << "Your answer (enter A/B/C/D): ";
    
    string playerAnswer;
    getline(cin, playerAnswer);
    playerAnswer = trim(playerAnswer);
    
    if (playerAnswer.length() != 1 || 
        (toupper(playerAnswer[0]) != 'A' && toupper(playerAnswer[0]) != 'B' && toupper(playerAnswer[0]) != 'C' && toupper(playerAnswer[0]) != 'D')) {
        cout << "✗ Invalid input! Please enter A, B, C, or D." << endl;
        cout << "The correct answer is: " << selectedQuestion.answer << endl;
        
        double actualPenalty = basePenalty;
        if (enemyType == 'T') {
            actualPenalty *= difficulty.ta_penalty_k;
        } else if (enemyType == 'F') {
            actualPenalty *= difficulty.prof_penalty_k;
        } else if (enemyType == 'S') {
            actualPenalty *= difficulty.stu_penalty_k;
        }
        
        cout << "You lost " << actualPenalty << " GPA!" << endl;
        return actualPenalty;
    }
    
    char playerChar = toupper(playerAnswer[0]);
    char correctChar = toupper(selectedQuestion.answer[0]);
    
    if (playerChar == correctChar) {
        cout << "✓ Correct! Well done!" << endl;
        return 0.0;
    } else {
        double actualPenalty = basePenalty;
        
        if (enemyType == 'T') {
            actualPenalty *= difficulty.ta_penalty_k;
        } else if (enemyType == 'F') {
            actualPenalty *= difficulty.prof_penalty_k;
        } else if (enemyType == 'S') {
            actualPenalty *= difficulty.stu_penalty_k;
        }
        
        cout << "✗ Wrong! The correct answer is: " << selectedQuestion.answer << endl;
        cout << "You lost " << actualPenalty << " GPA!" << endl;
        
        return actualPenalty;
    }
}

