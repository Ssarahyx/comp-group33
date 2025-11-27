# 🎓 HKU GPA ESCAPE 🏃‍♂️

A thrilling terminal-based survival game where you navigate through academic challenges as a HKU student trying to escape a building filled with "academic zombies" while maintaining your GPA! 📚➡️🎓

## 1️⃣ GAME OVERVIEW
HKU GPA Escape is a turn-based strategy game where you play as a HKU student trying to escape from a building while avoiding various academic entities. Your goal is to reach the exit on each level while answering questions correctly to maintain your GPA. 🎯🗺️

## 2️⃣ TEAM MEMBERS🧑‍💻
Si Xinwei （3036450732）

Wang Xianghe (3036445335)

Tong Bingchen (3036440775)

Song Yuxuan (3036290792)

## 3️⃣ GAME FEATURES

### 🎚️ 3 DIFFICULTY LEVELS
Easy, Normal, and Hard with different starting GPAs

### 📈 3 PROGRESSIVE LEVELS
Each level increases in complexity

### 👥 MULTIPLE ENEMY TYPES
TAs, Professors, and Students with different behaviors

### ❓ QUESTION SYSTEM
Answer C++ programming questions to defeat enemies

### 📊 GPA MANAGEMENT
Maintain your GPA above zero to survive

### 💾 SAVE/LOAD SYSTEM
Save your progress and resume later

### 🗺️ DYNAMIC MAP GENERATION
Randomly generated maps for replayability

## 4️⃣ HOW TO PLAY

### 🎯 BASIC CONTROLS
**W/A/S/D** - Move character 🎮

**P** - Save game progress 💾

**Enter** - Select menu options ✅

## 5️⃣ ACADEMIC CHALLENGES

### 👨‍🏫 TA ENEMIES (T)
Moderate C++ questions with strategic movement 🧠

65% chance to chase player, 35% random movement 🎲

### 👴 PROFESSOR ENEMIES (F)
Advanced programming concepts with aggressive pursuit 🔥

Always chases player directly 🎯

### 👨‍🎓 STUDENT ENEMIES (S)
Quick riddles with random movement patterns 🎪

Always moves randomly 🎲

## 6️⃣ GPA SYSTEM

### 🟢 EASY MODE
Starting GPA: 4.0 🎓

Reduced penalties and smaller maps

### 🟡 NORMAL MODE
Starting GPA: 3.5 📚

Balanced challenges and medium maps

### 🔴 HARD MODE
Starting GPA: 3.0 ⚡

Increased penalties and larger maps

**GPA Rules**
· GPA decreases when answering incorrectly
· Correct answers do not increase GPA but remove threats
· GPA reaching 0.0 results in Game Over

## 6️⃣ TECHNICAL FEATURES

### 🎲 RANDOM GENERATION
Dynamic Map Generation - Unique maps for each playthrough 🗺️

Random Enemy Placement - Enemies spawn in different locations 🎯

Question Randomization - Questions selected randomly from pools ❓

### 💾 DATA MANAGEMENT
GameState Class - Comprehensive game state management 🏗️

Entity System - Flexible character and enemy representation 👥

Save System - Persistent game progress storage 💾

### 📁 FILE SYSTEM
Question Files - External question database loading 📚

Configuration Files - Game settings management ⚙️

Save Files - Automatic progress backup 💾


## 7️⃣ CODING REQUIREMENTS IMPLEMENTATION
### 🎲 Requirement 1: Generation of Random Events
**Supported Game Features:**
- Dynamic Map Generation - Creates unique map layouts for each playthrough
- Random Enemy Placement - Enemies spawn in different locations each game
- Question Randomization - Questions selected randomly from question pools
- Enemy AI Behavior - TAs have 65% chance to chase player, 35% random movement

### 🗃️ Requirement 2: Data Structures for Storing Data
**Supported Game Features:**
- GameState Class - Stores game state, player position, GPA data
- Entity System - Manages data structures for players and various enemies
- Map Grid - Uses 2D array structures to store map layouts
- Question Database - Stores and manages C++ programming questions

### 💾 Requirement 3: Dynamic Memory Management
**Supported Game Features:**
- Dynamic Map Allocation - Creates different sized maps based on difficulty level
- Enemy Object Management - Runtime dynamic creation and management of enemy entities
- Game State Management - Dynamic allocation of game resources
- Question System - Dynamic loading and management of question data

### 📁 Requirement 4: File Input/Output
**Supported Game Features:**
- Save/Load System - Saves game progress to files and restores from files
- Question Database Reading - Loads C++ programming questions from external files
- Configuration Files - Manages game settings and difficulty parameters
- Automatic Backup - Ensures game progress is not lost

### 📄 Requirement 5: Program Codes in Multiple Files
**Supported Game Features:**
- Modular Architecture - Separates game logic into different .h and .cpp files
- Maintainability - Clear code organization for easier debugging and extension
- Team Collaboration - Allows team members to develop different modules in parallel
- Code Reusability - Independent components can be reused across different parts
  
## 8️⃣ EXTERNAL LIBRARIES

### Non-Standard Libraries
**None** - This project uses only C++ standard libraries, requiring no additional dependencies, ensuring direct compilation in the grader's environment.

## 9️⃣ INSTALLATION

### 🔧 Build the Game
```bash
make
```
This command compiles all source files and generates the executable:

```bash
hku_gpa_escape
```

### ▶ Run the Game
You can run the game in two different ways.

**Method 1**
Build first, then run manually
```bash
make
./hku_gpa_escape
```
**Method 2**
Compile (if needed) and run in a single step
```bash
make run
```

## 9️⃣ Quick Demo

https://github.com/user-attachments/assets/724b2d88-a1db-4806-9b22-f45d4e04dc0f


