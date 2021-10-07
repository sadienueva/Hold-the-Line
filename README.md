# Hold-the-Line Game Prototype (C++)

This project implements a simple 2D game prototype using C++ and SFML.

## Overview
* Player entity with movement, sprite handling, and attack cooldown logic
* Health and damage system for player and enemies
* Collision detection with player projectiles
* Spawn logic and simple AI loops
  
![GameScreenshot](https://user-images.githubusercontent.com/89870207/136309886-9ecd4376-8964-4fb4-bc01-c081d1f40438.PNG)

## Installation
1. Install SFML from https://www.sfml-dev.org/download.php  
Choose the version that matches your compiler (MinGW, Visual Studio, Clang, etc.).
2. Link SFML to Your Project
Example (macOS / Homebrew):
```bash
g++ main.cpp MainPlayer.cpp Enemy.cpp -o game \
-I/opt/homebrew/include -L/opt/homebrew/lib \
-lsfml-graphics -lsfml-window -lsfml-system
```
3. Run the game!
```
./game
```

## Players
* Movement: WASD directional controls
* Attack Cooldown: canAttack() throttles firing/attack rate
* Health System: HP / HPMAX, with damage handling via loseHP()
* Rendering: Player sprite loaded from texture and drawn each frame
* Position Utilities: Get/set functions for collision / movement coordination

## Dependencies
SFML 2.x (graphics, window, system modules)