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

// One-ply minimax around linear evaluation to reduce blunders.
// Evaluates from X perspective: X maximizes, O minimizes.
static inline int compute_best_move_depth1(int state[BOARD], float weights[FEATURES], int player)
{
    float best_val = (player == ML_X) ? -FLT_MAX : FLT_MAX;
    int best_move = -1;

    // Try each legal move for current player
    for (int i = 0; i < BOARD; i++) {
        if (state[i] != 0) continue;

        // Make candidate move
        state[i] = player;
        int opp = (player == ML_X) ? ML_O : ML_X;

        // Opponent chooses a reply that is best for them (max for X, min for O)
        float reply_val = (opp == ML_X) ? -FLT_MAX : FLT_MAX;
        int any_reply = 0;
        for (int j = 0; j < BOARD; j++) {
            if (state[j] != 0) continue;
            any_reply = 1;
            state[j] = opp;
            get_board_features(state, ML_X);
            float v = Eval_Approx(board_feature, weights);
            state[j] = 0;
            if (opp == ML_X) {
                if (v > reply_val) reply_val = v;
            } else {
                if (v < reply_val) reply_val = v;
            }
        }

        // If no reply (terminal or full board), evaluate current position
        if (!any_reply) {
            get_board_features(state, ML_X);
            reply_val = Eval_Approx(board_feature, weights);
        }

        // Undo candidate move
        state[i] = 0;

        // Current player picks move based on X-perspective value after opponent's best reply
        if (player == ML_X) {
            if (reply_val > best_val) { best_val = reply_val; best_move = i; }
        } else {
            if (reply_val < best_val) { best_val = reply_val; best_move = i; }
        }
    }

    return best_move;
}

#endif // ML_SHARED_H
