#ifndef ML_SHARED_H
#define ML_SHARED_H

/*
  Shared ML utilities used by both training and runtime.
  Requirements:
   - Include a config header before this that defines BOARD and FEATURES
   - Provide an extern definition for `board_feature[FEATURES]` in each TU
*/

#include <stddef.h>
#include <float.h>

// The including translation unit must define this symbol
extern int board_feature[FEATURES];

static inline void get_board_features(int state[BOARD], int Cplayer)
{
    int x0 = 1, x1 = 0 , x2 = 0, x3 = 0, x4 = 0, x5 = 0, x6 = 0;

    int CBoard[3][3];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            CBoard[i][j] = state[i * 3 + j];
        }
    }

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

        if (player_row == 2 && empty_row == 1) x1++;
        if (player_col == 2 && empty_col == 1) x1++;
        if (opponent_row == 2 && empty_row == 1) x2++;
        if (opponent_col == 2 && empty_col == 1) x2++;
        if (player_row == 1 && empty_row == 2) x5++;
        if (player_col == 1 && empty_col == 2) x5++;
        if (player_row == 3) x6++;
        if (player_col == 3) x6++;
    }

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

        if (player_dia == 2 && empty_dia == 1) x1++;
        if (opponent_dia == 2 && empty_dia == 1) x2++;
        if (player_dia == 1 && empty_dia == 2) x5++;
        if (player_dia == 3) x6++;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) {
                if (CBoard[i][j] == Cplayer) x3++;
            } else if ((i == 0 && j == 0) || (i == 0 && j == 2) || (i == 2 && j == 0) || (i == 2 && j == 2)) {
                if (CBoard[i][j] == Cplayer) x4++;
            }
        }
    }

    board_feature[0] = x0;
    board_feature[1] = x1;
    board_feature[2] = x2;
    board_feature[3] = x3;
    board_feature[4] = x4;
    board_feature[5] = x5;
    board_feature[6] = x6;
}

static inline float Eval_Approx(int features[FEATURES], float weight[FEATURES])
{
    float value = 0.0f;
    for (int i = 0; i < FEATURES; i++) {
        value += features[i] * weight[i];
    }
    return value;
}


#endif // ML_SHARED_H
