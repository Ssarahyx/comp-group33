#include "question.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <limits>
#include <random>  // Added for std::shuffle
using namespace std;

// Global vectors storing questions for different enemy types
vector<Qs> taQs;      ///< Questions asked by TAs
vector<Qs> profQs;    ///< Questions asked by Professors 
vector<Qs> stuQs;     ///< Questions asked by Students

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

void initQsRandom() {
    // Removed srand(time(0)) - only keep one srand in the entire program
    
    static std::random_device rd;
    static std::mt19937 g(rd());

    if (!taQs.empty()) {
        std::shuffle(taQs.begin(), taQs.end(), g);
    }
    if (!profQs.empty()) {
        std::shuffle(profQs.begin(), profQs.end(), g);
    }
    if (!stuQs.empty()) {
        std::shuffle(stuQs.begin(), stuQs.end(), g);
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

    // Select question based on enemy type
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

    // Present question and get player input
    cout << "\n=== " << enemyName << " Encounter! ===" << endl;
    cout << selectedQuestion.questionText << endl;
    
    char playerAnswer;
    bool validInput = false;
    
    while (!validInput) {
        cout << "Your answer (enter A/B/C/D): ";
        cin >> playerAnswer;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        playerAnswer = toupper(playerAnswer);
        
        // Validate input format
        if (playerAnswer == 'A' || playerAnswer == 'B' || 
            playerAnswer == 'C' || playerAnswer == 'D') {
            validInput = true;
        } else {
            cout << "✗ Invalid input! Please enter A, B, C, or D." << endl;
        }
    }

    // Evaluate answer
    char correctChar = toupper(selectedQuestion.answer[0]);
    
    if (playerAnswer == correctChar) {
        cout << "✓ Correct! Well done!" << endl;
        return 0.0;
    } else {
        double actualPenalty = basePenalty;
        
        // Apply penalty with difficulty multiplier
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
