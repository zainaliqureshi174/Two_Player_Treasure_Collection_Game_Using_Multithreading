#include <iostream>
#include <ncurses.h>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

/***************************************************************
*                     Name: Muhammad Zain Ali                  *
*                      Roll Number: 22i-0562                   *
*                          Section: AI-D                       *
*                    Course: Operating Systems                 *
*                Submitted To: Ma'am Palwasha Zahid            *
****************************************************************/
// g++ Q.cpp -o Q -lncurses -pthread
// ./Q

int WIDTH;
int HEIGHT;
int player1Score = 0;  // Score for Player 1
int player2Score = 0;  // Score for Player 2
int player1Items = 0;  // Player 1 Items Collection
int player2Items = 0;  // Player 2 Items Collection

atomic<int> player1X(0);  			// Player 1's cursor X position
atomic<int> player1Y(0);  			// Player 1's cursor Y position
atomic<int> player2X(0);  			// Player 2's cursor X position
atomic<int> player2Y(HEIGHT - 1);   // Player 2 starts at the bottom-left corner
atomic<bool> running(true);         // Flag to control the drawing loop
atomic<bool> player1Turn(true);     // Control player turn

vector<pair<int, int>> treasures;   // Vector to hold positions of treasures

// Generating a Random Number using RollNumber-algorithm
void Generate_Random_no()
{
    srand(time(0));
    int randomNumber = 10 + std::rand() % 90;
    randomNumber *= 2;
    randomNumber = 220562 / randomNumber;
    randomNumber %= 25;
    if (randomNumber < 10)
    {
        randomNumber += 15;
    }

    WIDTH = randomNumber;
    HEIGHT = randomNumber;
}

// Function to generate random positions for the players
void Generate_Random_Position()
{
    player1X = rand() % WIDTH;
    player1Y = rand() % HEIGHT;
    player2X = rand() % WIDTH;
    player2Y = rand() % HEIGHT;
}

// Function to generate random treasures on the board
void Generate_Treasures(int numTreasures)
{
    treasures.clear();  // Clear previous treasures
    for (int i = 0; i < numTreasures; ++i)
    {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        treasures.push_back({x, y});
    }
}

// Check if a player has found a treasure and remove it
bool checkForTreasure(int player)
{
    if (player == 1)
    {
        for (auto it = treasures.begin(); it != treasures.end(); ++it)
        {
            if (it->first == player1X && it->second == player1Y)
            {
                treasures.erase(it);  // Remove the treasure from the board
                player1Score += 50;  // Increment Player 1 score
                player1Items += 1;   // Increment Player 1 items collected
                return true;
            }
        }
    }
    else if (player == 2)
    {
        for (auto it = treasures.begin(); it != treasures.end(); ++it)
        {
            if (it->first == player2X && it->second == player2Y)
            {
                treasures.erase(it);  // Remove the treasure from the board
                player2Score += 50;  // Increment Player 2 score
                player2Items += 1;   // Increment Player 2 items collected
                return true;
            }
        }
    }
    return false;
}

// Drawing the Game Board with Score Bar and Turn Indicator
void drawBoard()
{
    while (running)
    {
        clear();

        // Draw the game board
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (x == player1X && y == player1Y) {
                    mvprintw(y, x, "1");  // Player 1 cursor
                } else if (x == player2X && y == player2Y) {
                    mvprintw(y, x, "2");  // Player 2 cursor
                } else {
                    bool isTreasure = false;
                    // Check if the current position has a treasure
                    for (auto& treasure : treasures)
                    {
                        if (treasure.first == x && treasure.second == y)
                        {
                            mvprintw(y, x, "T");  // Display treasure
                            isTreasure = true;
                            break;
                        }
                    }

                    if (!isTreasure) {
                        mvprintw(y, x, "-");  // Empty board cell
                    }
                }
            }
        }

        // Display the score bar to the right of the board
        int scoreBarX = WIDTH + 2;  // Position score bar to the right of the board
        mvprintw(0, scoreBarX, "Score Bar");  // Title for score bar
        mvprintw(2, scoreBarX, "Player 1: %d", player1Score);  // Player 1 score
        mvprintw(3, scoreBarX, "Player 2: %d", player2Score);  // Player 2 score
        mvprintw(4, scoreBarX, "Player 1 Items Collected: %d", player1Items);  // Player 1 items
        mvprintw(5, scoreBarX, "Player 2 Items Collected: %d", player2Items);  // Player 2 items

        // Display turn indicator
        if (player1Turn) {
            mvprintw(6, scoreBarX, "Player 1 Turn: 0");  // Player 1's turn
            mvprintw(7, scoreBarX, "Player 2 Turn:");    // Player 2's turn
        } else {
            mvprintw(6, scoreBarX, "Player 1 Turn:");    // Player 1's turn
            mvprintw(7, scoreBarX, "Player 2 Turn: 0");  // Player 2's turn
        }

        refresh();
        this_thread::sleep_for(chrono::milliseconds(100));  // Adjust the delay to control refresh speed

        // If no treasures are left, end the game
        if (treasures.empty()) {
            running = false;
            mvprintw(HEIGHT / 2, WIDTH / 2, "GAME OVER");
            mvprintw(HEIGHT / 2 + 1, WIDTH / 2, "Player 1: %d, Player 2: %d", player1Score, player2Score);
            if (player1Score > player2Score) {
                mvprintw(HEIGHT / 2 + 2, WIDTH / 2, "Player 1 Wins!");
            } else if (player2Score > player1Score) {
                mvprintw(HEIGHT / 2 + 2, WIDTH / 2, "Player 2 Wins!");
            } else {
                mvprintw(HEIGHT / 2 + 2, WIDTH / 2, "It's a Tie!");
            }
            refresh();
            this_thread::sleep_for(chrono::seconds(2));  // Display the result for 2 seconds
        }
    }
}

// Moves the cursor within bounds and updates player scores
void moveCursor(int dx, int dy, int player)
{
    if (player == 1)
    {
        player1X = max(0, min(WIDTH - 1, player1X + dx));
        player1Y = max(0, min(HEIGHT - 1, player1Y + dy));
        if (checkForTreasure(1)) {
            mvprintw(HEIGHT, WIDTH + 2, "Player 1 found a treasure!");
        }
    }
    else if (player == 2)
    {
        player2X = max(0, min(WIDTH - 1, player2X + dx));
        player2Y = max(0, min(HEIGHT - 1, player2Y + dy));
        if (checkForTreasure(2)) {
            mvprintw(HEIGHT, WIDTH + 2, "Player 2 found a treasure!");
        }
    }
}

// Handle the turn for a specific player
void playerControl()
{
    while (running)
    {
        int ch = getch();  // Wait for a key press

        if (player1Turn) {
            // Player 1 controls
            switch (ch)
            {
                case KEY_UP:
                    moveCursor(0, -1, 1);
                    break;
                case KEY_DOWN:
                    moveCursor(0, 1, 1);
                    break;
                case KEY_LEFT:
                    moveCursor(-1, 0, 1);
                    break;
                case KEY_RIGHT:
                    moveCursor(1, 0, 1);
                    break;
                case 'q':  // Quit
                    running = false;
                    break;
            }
            player1Turn = false;
        }
        else {
            // Player 2 controls
            switch (ch)
            {
                case KEY_UP:
                    moveCursor(0, -1, 2);
                    break;
                case KEY_DOWN:
                    moveCursor(0, 1, 2);
                    break;
                case KEY_LEFT:
                    moveCursor(-1, 0, 2);
                    break;
                case KEY_RIGHT:
                    moveCursor(1, 0, 2);
                    break;
                case 'q':  // Quit
                    running = false;
                    break;
            }
            player1Turn = true;
        }
    }
}

int main()
{
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    Generate_Random_no();
    Generate_Random_Position();
    Generate_Treasures(5);  // Generate 5 treasures

    // Start drawing and player controls in separate threads
    thread drawingThread(drawBoard);
    thread controlThread(playerControl);

    // Wait for both threads to finish
    drawingThread.join();
    controlThread.join();

    // End ncurses mode
    endwin();
    return 0;
}

