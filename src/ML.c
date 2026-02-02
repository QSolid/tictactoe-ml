#include "ML-config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "ml_shared.h"

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

// get_board_features and Eval_Approx are provided by ml_shared.h


// Get the best move the AI can make using the trained weights
void BestMove(int state[BOARD],float weights[FEATURES],int player)
{
    // Pure-ML one-ply: pick move maximizing Eval_Approx for `player`
    int best_index = -1;
    float best_score = -FLT_MAX;

    for (int i = 0; i < BOARD; i++) {
        if (state[i] != ML_EMPTY) continue;

        int saved = state[i];
        state[i] = player; // simulate move in ML encoding

        get_board_features(state, player);
        float score = Eval_Approx(board_feature, weights);

        if (score > best_score) {
            best_score = score;
            best_index = i;
        }

        state[i] = saved;
    }

    if (best_index != -1) {
        ml_board_state[best_index] = player;
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
