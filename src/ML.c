#include "ML-config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// External variables from main.c for GTK integration
extern GtkWidget *button11;
extern GtkWidget *button12;
extern GtkWidget *button13;
extern GtkWidget *button21;
extern GtkWidget *button22;
extern GtkWidget *button23;
extern GtkWidget *button31;
extern GtkWidget *button32;
extern GtkWidget *button33;
extern GtkWidget *modebutton;
extern GtkLabel *gamemode;
extern GtkLabel *game_stats;
extern GtkLabel *statusbtn;
extern int arr[3][3];
extern int pressed[3][3];
extern int moveCounter;
extern int gameNotOver;
extern int gameDifficulty;

// Global ML data structures                                        
int board_feature[FEATURES];                        // Feature array
int ml_board_state[BOARD] = {0};                    // ML board representation
float init_weight[FEATURES];                        // Initial weights

int moves[BOARD][BOARD +1];                         // Possible moves matrix 

// Initialise ML system
void init_ml_system() 
{
    // Initialise weights
    readweights();
}

void get_board_features(int state[BOARD],int Cplayer) // Set board features; used with weights to train ML
{
    /*
        x0 = 1  # Constant
        x1 = 0  # Number of rows/columns/diagonals with two of our own pieces and one empty field
        x2 = 0  # Number of rows/columns/diagonals with two of opponent's pieces and one empty field
        x3 = 0  # Is our own piece on the center field
        x4 = 0  # Number of own pieces in corners
        x5 = 0  # Number of rows/columns/diagonals with one own piece and two empty fields
        x6 = 0  # Number of rows/columns/diagonals with three own pieces
    */
    int x0 = 1, x1 = 0 , x2 = 0, x3 = 0, x4 = 0, x5 = 0, x6 = 0;

    // Convert 1D array to a 2D board for easier processing
    int CBoard[3][3];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            CBoard[i][j] = state[i * 3 + j];
        }
    }

    // Count player, opponent, and empty cells in rows and columns
    for (int i = 0; i < 3; i++) {
        int player_row = 0, player_col = 0, opponent_row = 0, opponent_col = 0, empty_row = 0, empty_col = 0;
        for (int j = 0; j < 3; j++) {
            if (CBoard[i][j] == Cplayer) {
                player_row++;
            } else if (CBoard[i][j] == 0) {
                empty_row++;
            } else {
                opponent_row++;
            }

            if (CBoard[j][i] == Cplayer) {
                player_col++;
            } else if (CBoard[j][i] == 0) {
                empty_col++;
            } else {
                opponent_col++;
            }
        }

        // Update feature values
        if (player_row == 2 && empty_row == 1) {
            x1++;
        }
        if (player_col == 2 && empty_col == 1) {
            x1++;
        }
        if (opponent_row == 2 && empty_row == 1) {
            x2++;
        }
        if (opponent_col == 2 && empty_col == 1) {
            x2++;
        }
        if (player_row == 1 && empty_row == 2) {
            x5++;
        }
        if (player_col == 1 && empty_col == 2) {
            x5++;
        }
        if (player_row == 3) {
            x6++;
        }
        if (player_col == 3) {
            x6++;
        }
    }
        // Check diagonals
    for (int i = 0; i < 2; i++) {
        int player_dia = 0, empty_dia = 0, opponent_dia = 0;
        for (int j = 0; j < 3; j++) {
            int temp = (i == 0) ? CBoard[2 - j][j] : CBoard[j][j];
            if (temp == Cplayer) {
                player_dia++;
            } else if (temp == 0) {
                empty_dia++;
            } else {
                opponent_dia++;
            }
        }

        if (player_dia == 2 && empty_dia == 1) {
            x1++;
        }
        if (opponent_dia == 2 && empty_dia == 1) {
            x2++;
        }
        if (player_dia == 1 && empty_dia == 2) {
            x5++;
        }
        if (player_dia == 3) {
            x6++;
        }
    }

    // Check center and corners
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) {
                if (CBoard[i][j] == Cplayer) {
                    x3++;
                }
            } else if ((i == 0 && j == 0) || (i == 0 && j == 2) || (i == 2 && j == 0) || (i == 2 && j == 2)) {
                if (CBoard[i][j] == Cplayer) {
                    x4++;
                }
            }
        }
    }
    
    // Set features
    board_feature[0] = x0;
    board_feature[1] = x1;
    board_feature[2] = x2;
    board_feature[3] = x3;
    board_feature[4] = x4;
    board_feature[5] = x5;
    board_feature[6] = x6;
}

// Evaluate and assign value to the board state
float Eval_Approx(int features[FEATURES], float weight[FEATURES])
{
    float value =0;
    for(int i=0; i<FEATURES;i++)
    {
        value += features[i] * weight[i];
    }
    return value;
}


// Get the best move the AI can make using the trained weights
void BestMove(int state[BOARD],float weights[FEATURES],int player)
{
    float best_val = (player == ML_X) ? -FLT_MAX : FLT_MAX; // Maximize for X, minimize for O
    int best_move = -1;     // Track position of best move found
    
    // Single pass through the board to find the best move
    for(int i = 0; i < BOARD; i++) {
        if(state[i] == 0) {  // If empty space
            // Make move
            state[i] = player;
            
            // Calculate features from X perspective and evaluate
            get_board_features(state, ML_X);
            float val = Eval_Approx(board_feature, weights);
            
            // Undo move
            state[i] = 0;
            
            // Update best based on current player
            if (player == ML_X) {
                if (val > best_val) { best_val = val; best_move = i; }
            } else { // ML_O
                if (val < best_val) { best_val = val; best_move = i; }
            }
        }
    }
    
    // Make final move
    if(best_move != -1) {
        ml_board_state[best_move] = player;
    }
}

void readweights()
{
    float weight_values =0;
    FILE *Read_weights = fopen(WEIGHTS,"r");
    if(Read_weights == NULL)
    {
        printf("File does not exist");
        exit(1);
    }

    for(int i =0; i < 7;i++)
    {
        fscanf(Read_weights,"%f",&weight_values);
        init_weight[i] =weight_values;
    }
    fclose(Read_weights);
}
