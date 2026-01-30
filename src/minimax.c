#include "minimax.h" //header file
#include <stdbool.h> // allows usage of Bool
#include <limits.h> // For INT_MIN and INT_MAX

// Declare extern used because this are from main.c
// Extern variables are used because their definitions lie elesewhere
extern GtkButton *statusClick;
extern GtkButton *button11;
extern GtkButton *button12;
extern GtkButton *button13;
extern GtkButton *button21;
extern GtkButton *button22;
extern GtkButton *button23;
extern GtkButton *button31;
extern GtkButton *button32;
extern GtkButton *button33;
extern GtkButton *modebutton;
extern GtkLabel *gamemode;
extern GtkLabel *statusbtn;

extern int arr[3][3];
extern int pressed[3][3];
extern int moveCounter;
extern int gameNotOver;
extern int gameDifficulty;

int maxDepth = 0; // Global variable to control minimax depth

//Evaluates the current board state and returns a score.
int evaluate(int board[3][3]) 
{
   //Checking rows and column for win
    for (int i = 0; i < 3; i++) 
    {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) 
        {
            if (board[i][0] == COMPUTER) return 9;
            if (board[i][0] == PLAYER) return -9;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) 
        {
            if (board[0][i] == COMPUTER) return 9;
            if (board[0][i] == PLAYER) return -9;
        }
    }
    // Checking diagonals for win
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) 
    {
        if (board[0][0] == COMPUTER) return 9;
        if (board[0][0] == PLAYER) return -9;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) 
    {
        if (board[0][2] == COMPUTER) return 9;
        if (board[0][2] == PLAYER) return -9;
    }
    return 0; //return draw
}

//Checks if there are any empty cells left on the board.
int isMovesLeft(int board[3][3]) 
{
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (board[i][j] == EMPTY) return 1;
        }
    }
    return 0;
}

// minimax is recusively called
int minimax(int board[3][3], int depth, int isMaximizingai) 
{
    int score = evaluate(board);
    // Choosing 9 or -9 is enough as the maxdepth is 8 or -8 due to edge cases score of 9 will be used
    if (score == 9) return score - depth;
    if (score == -9) return score + depth;
    // Stops the checking if there is no moves anymore || If depth = maxdepth stop running
    if (!isMovesLeft(board) || depth == maxDepth) return 0;
    // isMaximisingai is 1 and 0, 1 =  indicating AI, 0 = Player move
    if (isMaximizingai) 
    {
        int best = INT_MIN; //Ai starts with the lowest score
        for (int i = 0; i < 3; i++) 
        {
            for (int j = 0; j < 3; j++) 
            {
                if (board[i][j] == EMPTY) 
                {
                    board[i][j] = COMPUTER; //Ai move
                    int moveVal = minimax(board, depth + 1, 0); // Store the current recursion score
                    best = (moveVal > best) ? moveVal : best; // Store the result if higher than current "best"
                    board[i][j] = EMPTY; //Clear the move for next recursion
                }
            }
        }
        return best; //The highest possible score within 9
    } 
    else 
    {
        int best = INT_MAX; //Player starts with the highest score
        for (int i = 0; i < 3; i++) 
        {
            for (int j = 0; j < 3; j++) 
            {
                if (board[i][j] == EMPTY) 
                {
                    board[i][j] = PLAYER;  //Player simulated move
                    int moveVal = minimax(board, depth + 1, 0); // Store the current recursion score
                    best = (moveVal < best) ? moveVal : best; // store the result if lower than current "best"
                    board[i][j] = EMPTY; //Clear the move for next recursion
                }
            }
        }
        return best; //The closest possible score to -9
    }
}
//Checks if the game has ended and updates the game status accordingly.
void checkGameStatus() 
{
    int score = evaluate(arr);
    if (score == 9) {
        update_game_status("COMPUTER WON", 2);
        play_sound("lose.wav");
    } else if (score == -9) {
        update_game_status("PLAYER 2 WON", 1);
    } else if (!isMovesLeft(arr)) {
        update_game_status("DRAW", 0);
    }
}

//Function created to pick empty squares for random function
bool selRandomEmptyCell(int *row, int *col) 
{
    //Declaring all spaces in the board
    int emptyCells[9][2];
    int emptyCount = 0;
    // Get all empty squares
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (arr[i][j] == EMPTY) 
            {
                emptyCells[emptyCount][0] = i;
                emptyCells[emptyCount][1] = j;
                emptyCount++;
            }
        }
    }

    // Random function
    if (emptyCount > 0) 
    {
        int randomIndex = rand() % emptyCount; // modulus emptyCount would ensure that it will pick from within the no. of empty cells remaining
        *row = emptyCells[randomIndex][0];
        *col = emptyCells[randomIndex][1];
        return true;
    }

    return false;
}

// makeMove whenever Computer makes a move on the board
void makeMove(int row, int col) 
{
    //Call the buttons
    GtkButton *buttons[3][3] = {
        {GTK_BUTTON(button11), GTK_BUTTON(button12), GTK_BUTTON(button13)},
        {GTK_BUTTON(button21), GTK_BUTTON(button22), GTK_BUTTON(button23)},
        {GTK_BUTTON(button31), GTK_BUTTON(button32), GTK_BUTTON(button33)}
    };

    // Update the game board and UI
    arr[row][col] = COMPUTER;
    gtk_button_set_label(buttons[row][col], "O");
    gtk_widget_set_name(GTK_WIDGET(buttons[row][col]), "player_2");
    pressed[row][col] = 1;
    moveCounter++; //Increment of counter to ensure that the game always know whos turn it is as there is no button click
    checkGameStatus();
}

//Handles the Computer move logic using minimax.
void compMove() 
{
    int row, col;

    // Easy difficulty uses just the random function
    if (gameDifficulty == 0) 
    {
        if (selRandomEmptyCell(&row, &col)) // Allow the function to manipulate the row and col directly
        {
            makeMove(row, col); // execute the move
        }
        return;
    }

    // Only the first move is made by the random function for Medium and Hard
    if ((gameDifficulty == 1 || gameDifficulty == 2) && (moveCounter ==1)) 
    {
         if (selRandomEmptyCell(&row, &col)) 
        {
            makeMove(row, col);
        }
        return;
    }

    // Set maxDepth based on difficulty level for minimax
    maxDepth = (gameDifficulty == 1) ? 1 : 9; // Medium: 1; Hard: 9 (full depth)
    
    int bestVal = INT_MIN;
    int bestRow = -1, bestCol = -1;

    // Recursively trigger minimax for all the moves
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (arr[i][j] == EMPTY) 
            {
                arr[i][j] = COMPUTER; //Place Comp move
                int moveVal = minimax(arr, 0, 0); //trigger minimax
                arr[i][j] = EMPTY; // Clear moves

                //Evaluate if the move is better or not
                if (moveVal > bestVal) 
                {
                    bestRow = i;
                    bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    // Place "O" on the best position using make move funtion
    if (bestRow != -1 && bestCol != -1) 
    {
        makeMove(bestRow, bestCol);
    }
}
