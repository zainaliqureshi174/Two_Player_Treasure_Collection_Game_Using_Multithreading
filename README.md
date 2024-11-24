# Two_Player_Treasure_Collection_Game_Using_Multithreading

This is a simple terminal-based two-player treasure hunt game built in C++ using the ncurses library. Players compete to collect treasures on a randomly generated board.

**Features:**
Random board size and player positions.
Treasures are randomly placed on the board.
Turn-based gameplay: players alternate turns to move and collect treasures.
Real-time score display.
Threaded architecture for smooth game experience.

**Controls:**
Player 1: Uses Arrow keys to move.
Player 2: Uses Arrow keys (after Player 1's turn).
Quit: Press q to exit the game.

**How to Compile and Run:**
Install ncurses (if not already installed):

sudo apt-get install libncurses5-dev libncursesw5-dev

**Compile the code:**

g++ Q.cpp -o Q -lncurses -pthread

**Run the game:**

./Q

**Dependencies:**
C++ compiler (g++)
ncurses library
pthread library

**Gameplay Instructions:**
Launch the game.
Players take turns moving their cursor to collect treasures.
The player with the highest score at the end of the game wins.

**Winning Conditions:**
All treasures are collected.
The game displays the winner or a tie if scores are equal.
