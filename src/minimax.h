#ifndef MINIMAX_H
#define MINIMAX_H

#include <gtk/gtk.h> // for GTK
#include <limits.h> // For INT_MIN and INT_MAX

// Constants for players and empty cells
#define PLAYER 1
#define COMPUTER 2
#define EMPTY 0

// Function prototypes
int evaluate(int board[3][3]);
int isMovesLeft(int board[3][3]);
int minimax(int board[3][3], int depth, int isMaximizingai);
void checkGameStatus();
void compMove();
void update_game_status(const char *status_text, int winner);
void play_sound(const char *filepath);

#endif
