# Terminal-Based Chess Game in C++

## Overview
This is a terminal-based chess game implemented in C++. It features a graphical chessboard displayed using ANSI escape codes and Unicode chess symbols, allowing two players to play chess locally via keyboard input. The game includes legal move validation, special chess rules (castling, pawn promotion), check/checkmate/stalemate detection, and sound effects.

## Features
- Chessboard rendered in terminal with colored squares and chess pieces using Unicode.
- Cursor navigation via keyboard (WASD keys, numeric keypad).
- Full chess rules support including:
  - Pawn promotion
  - Castling (king and rook special move)
  - Check, checkmate, stalemate, and draw detection
- Sound effects for moves, checks, checkmate, and game over states.
- Board theme customization 
- Turn-based gameplay for two human players.
- Lightweight and runs on Linux terminals.

## Requirements
- A Linux system with a terminal emulator.
- C++ compiler supporting C++11 or higher.


## How to Run
1. Clone or download the project.
2. Compile the code using a C++ compiler, for example: g++ -o chess_game main.cpp
3. Run the executable: ./chess_game
4. Select start
5. Use the keyboard to move the cursor and select pieces/moves:
- w, a, s, d or 8, 4, 2, 6 for navigation
- Enter key to select piece or move

## Controls
- Move cursor: W/A/S/D or numeric keypad 8/4/2/6
- Select piece or confirm move: Enter
- Diagonal moves using Q/E/Z/C or 7/9/1/3 keys for faster navigation.

## License
This project is for educational purposes and is not currently licensed for redistribution.