# 🎓 HKU GPA ESCAPE 🏃‍♂️

A thrilling terminal-based survival game where you navigate through academic challenges as a HKU student trying to escape a building filled with "academic zombies" while maintaining your GPA! 📚➡️🎓

## 1️⃣ GAME OVERVIEW
HKU GPA Escape is a turn-based strategy game where you play as a HKU student trying to escape from a building while avoiding various academic entities. Your goal is to reach the exit on each level while answering questions correctly to maintain your GPA. 🎯🗺️

## 2️⃣ TEAM MEMBERS🧑‍💻
Si Xinwei


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
**W/A/S/D** or **↑/↓/←/→** - Move character 🎮

**P** - Save game progress 💾

**Enter** - Select menu options ✅

### 💡 PRO TIPS
Plan your route to avoid unnecessary enemy encounters 🗺️

Save frequently to protect your GPA progress 💾

Different enemies require different strategies 🎯

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


## 7️⃣ QUICK START
Select difficulty level 🎚️

Navigate through the map using WASD keys 🗺️

Avoid or confront enemies when encountered 👥

Answer questions correctly to deactivate enemies ✅

Reach the exit (E) to complete each level 🚪

Save your progress regularly with P key 💾

Complete all 3 levels to win the game! 🏆

## 8️⃣ INSTALLATION

### 🔧 Build the Game
```bash
make
```
This command compiles all source files and generates the executable:
```bash
hku_gpa_escape```

### ▶ Run the Game
To build (if needed) and run the game in one step:
```bash
make run
```
This executes the compiled binary automatically.


