#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief Structure representing game difficulty settings
 * @param initialGPA Starting GPA value for the game
 * @param ta_penalty_k Penalty multiplier for TA encounters
 * @param prof_penalty_k Penalty multiplier for Professor encounters  
 * @param stu_penalty_k Penalty multiplier for Student encounters
 */
struct set_difficulty {
    double initialGPA;
    double ta_penalty_k;
    double prof_penalty_k;
    double stu_penalty_k;
};

/**
 * @brief Structure representing a question in the game
 * @param questionText The text of the question to be displayed
 * @param answer The correct answer (A, B, C, or D)
 * @param basePenalty The base GPA penalty for incorrect answers
 */
struct Qs {
    string questionText;
    string answer;
    double basePenalty;
};

// Global vectors storing questions for different enemy types
extern vector<Qs> taQs;      ///< Questions asked by TAs
extern vector<Qs> profQs;    ///< Questions asked by Professors 
extern vector<Qs> stuQs;     ///< Questions asked by Students

// Function declarations

/**
 * @brief Removes leading and trailing whitespace from a string
 * @param str Input string to be trimmed
 * @return Trimmed string without leading/trailing whitespace
 */
string trim(const string& str);

/**
 * @brief Initializes random question ordering using modern shuffle algorithm
 * Uses std::shuffle for proper random distribution instead of unstable sort
 */
void initQsRandom();

/**
 * @brief Loads all question files (TA, Professor, Student)
 */
void load_All_Qs();

/**
 * @brief Loads TA questions from specified file
 * @param filename Path to the TA questions file
 */
void load_TA_Qs(const string& filename);

/**
 * @brief Loads Professor questions from specified file
 * @param filename Path to the Professor questions file
 */
void load_Prof_Qs(const string& filename);

/**
 * @brief Loads Student questions from specified file
 * @param filename Path to the Student questions file
 */
void load_Stu_Qs(const string& filename);

/**
 * @brief Presents a question to the player and evaluates their answer
 * @param enemyType Character representing enemy type ('T'=TA, 'F'=Professor, 'S'=Student)
 * @param difficulty Difficulty settings for penalty calculations
 * @return GPA penalty applied (0.0 if answer was correct)
 */
double ask(char enemyType, const set_difficulty& difficulty);

#endif
