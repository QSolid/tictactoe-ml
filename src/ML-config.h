#ifndef ML_CONFIG_H
#define ML_CONFIG_H

#include <gtk/gtk.h>

/* Prototypes and global variables used in ML.c */
#define BOARD 9                             // Grids of tic-tac-toe board
#define FEATURES 7                          // Number of features 
#define WEIGHTS "models/MLWeights.txt"

// ML player constants (avoid magic numbers)
#define ML_EMPTY 0
#define ML_X 1
#define ML_O -1

/* External declarations of global variables for main.c */
extern int ml_board_state[BOARD];           // Declaration of ML's board state
extern float init_weight[FEATURES];         // Declaration of initial weights

/* ML Prototypes */
void init_ml_system(void); // Initialise ML functions
void get_board_features(int state[BOARD],int Cplayer); // Set board features; used with weights to train ML
float Eval_Approx(int features[FEATURES], float weight[FEATURES]); // Evaluate and assign value to board state
void BestMove(int state[BOARD],float weights[FEATURES],int player); // Get the best move the AI can make using the trained weights
void readweights();
#endif
