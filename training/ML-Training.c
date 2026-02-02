#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include "Training-config.h"

/*
    ML training using linear regression, with comparison functions such as:
        - Classification metrics: Mean Squared Error (MMSE), Accuracy, True Positives/Negatives, False Positives/Negatives
        - Play against Minimax algorithm
        - Play against random inputs
    - Creates a text file that contains the 7 weights used in the main program
    - Ability to play against the trained model through the CLI
*/
int main()
{
    extract(DATA_FILE,dataset);
    cat_data();
    split_data();
    printf("Starting training..."); 
    trainModel(training_dataset,init_weight);
    testModel(testing_dataset,init_weight);
    //TrainwithItself(EPOCH,init_weight); unused function that trains by playing with itself

  
    printf("\nWeights : ");
    for(int i=0;i < FEATURES;i++)
    {
        printf("%f ",init_weight[i]);
    }
    printf("\n");
    int choice = -1;
    //choice of what to do we ML
    while (choice != 1 && choice != 2 && choice !=3 && choice !=4)
    {
        printf("Select one of the following:\n1) Evaluate against MinMax (1000 games)\n2) Evaluate against Random (1000 games)\n3) Play against ML\n4) Exit\nEnter a choice (1-4): ");
        scanf("%d",&choice);
    }

    if(choice == 1)
    {
        MLvsMinMAx(GAME_LENGTH);
    }
    else if(choice ==2)
    {
        MLvsRandom(GAME_LENGTH);
    }
    else if(choice ==3)
    {
        playAgainstML();
    }
    else if(choice ==4)
    {
        exit(0);
    }
}


// Extract and place into array
void extract(char *file_p, int a[SIZE][BOARD + 1])
{
    FILE *data;
    char buffer[200];
    int count = 0;
    data = fopen(file_p,"r"); // Open file and read file
    if(data == NULL)
    {
        printf("error reading file, maybe file path is wrong");
        exit(1);
    }


    while(fgets(buffer,sizeof(buffer),data))
    {
        
        // Split string by comma
        char *token = strtok(buffer,",");
        // Iterate inner array to store values from the line
        int i =0;
        while(token)
        {
            // If token only contains a single char add to x else add to y
            a[count][i] = convert_input(token);
            i++;

            // Increment count when a full line of values has been stored
            if(i == 10)
            {
                // Debug: print stored data
                // for(int j = 0; j < i;j++)
                // {
                //     printf("x[%d][%d] = %d\n", count,j, a[count][j]);
                // }
                count++;
            }

            token=strtok(NULL,",");
        }

    }

    fclose(data); // Close file
}


// Converts x, o, b, +ve and -ve into integers for linear regression
int convert_input(const char *input)
{
    if (strcmp(input, "positive\n") == 0) 
    {
        return 1;
    }
     else if (strcmp(input, "negative\n") == 0) 
    {
        return -1;
    } 
    // If input only contain single char return these values
    else if (strlen(input) == 1) 
    {
        switch (input[0]) {
            case 'o':
                return -1;
            case 'x':
                return 1;
            case 'b':
                return 0;
            default:
                return 3; // Return 3 means something went wrong
        }
    }
    return 3;
}

// Check if board state: win, lose, draw, or running
int BoardState(int state[BOARD])
{
    int board_state =0;
    // Loop through legal wins
    for(int i =0; i< 8;i++)
    {
        if(state[legal_wins[i][0]] != 0 && state[legal_wins[i][0]] == state[legal_wins[i][1]] && state[legal_wins[i][0]] == state[legal_wins[i][2]])
        {
            board_state = state[legal_wins[i][2]];
            return board_state;
        }
    }
    // Check for blank cell; still running
    for (int i = 0; i < BOARD; i++){
        if (state[i] == 0){
            board_state = 9;
            return board_state; 
        }
    }

    return board_state;    
}

// Categorize data into win, loss, and draw
void cat_data()
{
    for(int i=0; i< SIZE; i++)
    {
        dataset[i][9] = BoardState(dataset[i]);
    }


    for(int i =0;i<SIZE;i++)
    {
        // Win
        if (dataset[i][9] == 1)
        {
            for(int j=0; j<= BOARD;j++)
            {
                win_data[wins][j] =dataset[i][j];
            }
            wins++;
        }
        // Lose
        else if(dataset[i][9] == -1)
        {
            for(int j=0;j<=BOARD;j++)
            {
                loss_data[lose][j] = dataset[i][j];
            }
            lose++;
        }
        // Draw
        else if (dataset[i][9] == 0)
        {
            for(int j=0;j<=BOARD;j++)
            {
                draw_data[draw][j] = dataset[i][j];
            }
            draw++;
        }
    }
    printf("Wins: %d Lose: %d Draws: %d", wins, lose, draw);


}

// Helper to shuffle class-specific arrays (top-level C function)
void shuffle_rows(int a[][BOARD + 1], int count)
{
    for(int i = count - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        // swap row i and j
        for(int k = 0; k <= BOARD; k++)
        {
            int tmp = a[i][k];
            a[i][k] = a[j][k];
            a[j][k] = tmp;
        }
    }
}

// Split data into training and testing
void split_data()
{
    // Seed RNG for shuffling
    srand((unsigned)time(NULL));

    // Randomise per-class to preserve class proportions in splits
    shuffle_rows(win_data, wins);
    shuffle_rows(loss_data, lose);
    shuffle_rows(draw_data, draw);

    // 80% split for training
    int Training_Wins = wins * 0.8;
    int Training_Lose = lose * 0.8;
    int Training_Draws = draw * 0.8;
    printf("\nTraining splits Wins: %d Lose: %d Draws: %d", Training_Wins, Training_Lose, Training_Draws);
    // 20% split for testing
    int Testing_Wins = wins - Training_Wins;
    int Testing_Lost = lose - Training_Lose;
    int Testing_Draw = draw - Training_Draws;
    printf("\nTesting splits Wins: %d Lose: %d Draws: %d\n", Testing_Wins, Testing_Lost, Testing_Draw);
    // Counters
    int winsC=0,loseC=0,drawC=0;

    // Add wins,lose,draw to training_dataset without readding the same element into test_data
    for(int i =0; i < TRAINING_SIZE;i++)
    {
        // Add wi lose draw to training dataset
        if(i < Training_Wins)
        {
            for(int j=0; j<= BOARD; j++)
            {
                training_dataset[i][j] = win_data[winsC][j];
            }
            winsC++;
        }
        else if(i < Training_Wins + Training_Lose)
        {
            for(int j=0; j<= BOARD; j++)
            {
                training_dataset[i][j] = loss_data[loseC][j];
            }    
            loseC++;        
        }
        else
        {
            for(int j=0; j<= BOARD; j++)
            {
                training_dataset[i][j] = draw_data[drawC][j];
            }
            drawC++;
        }
    }


    // Add wins,lose,draw to testing_dataset after finishing adding to training
    for(int i =0; i < TESTING_SIZE;i++)
    {
        // Add
        if(i < Testing_Wins)
        {
            for(int j=0; j<= BOARD; j++)
            {
                testing_dataset[i][j] = win_data[winsC][j];
            }
            winsC++;
        }
        else if(i < Testing_Wins + Testing_Lost)
        {
            for(int j=0; j<= BOARD; j++)
            {
                testing_dataset[i][j] = loss_data[loseC][j];
            }    
            loseC++;        
        }
        else
        {
            for(int j=0; j<= BOARD; j++)
            {
                testing_dataset[i][j] = draw_data[drawC][j];
            }
            drawC++;
        }
    }

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
        Snippet source: https://github.com/Skyy93/TicTacToeRegressionBot/blob/master/player.py
    */
    int x0=1,x1=0,x2=0,x3=0,x4=0,x5=0,x6=0;
    // Create a 2D grid representing the current board
    int CBoard[3][3];
    CBoard[0][0] = state[0];
    CBoard[0][1] = state[1];
    CBoard[0][2] = state[2];
    CBoard[1][0] = state[3];
    CBoard[1][1] = state[4];
    CBoard[1][2] = state[5];
    CBoard[2][0] = state[6];
    CBoard[2][1] = state[7];
    CBoard[2][2] = state[8];

    // Loop through the board row by row
    for(int row =0; row < 3;row++)
    {
    // Counters for player/opponent/empty in row/column
    int player_row =0,player_col =0,oppo_row=0,oppo_col =0, empty_row =0, empty_col=0;
    for (int col=0; col<3;col++)
    {
        //row count for all 3
        if(CBoard[row][col] == Cplayer)
        {
            player_row++;
        }
        else if(CBoard[row][col] == 0)
        {
            empty_row++;
        }
        else
        {
            oppo_row++;
        }

        //column count for all 3
        if(CBoard[col][row] == Cplayer)
        {
            player_col++;
        }
        else if(CBoard[col][row] == 0)
        {
            empty_col++;
        }
        else
        {
            oppo_col++;
        }

        //check x3 and x4
        if(row == col)
        {
            // Check center
            if(row == 1 && col == 1)
            {
                if(CBoard[row][col] == Cplayer)
                {
                    x3++;
                }
            }
            else
            {
                // Count how many pieces are in the corner
                if(CBoard[row][col] == Cplayer)
                {
                    x4++;
                }
            }
        }

        if(row + col ==2)
        {
            if (row != 1 && col != 1)
            {
                if(CBoard[row][col] == Cplayer)
                {
                    x4++;
                }
            }
        }

        // Check x1 rows and columns
        if(player_row == 2 && empty_row ==1)
        {
            x1++;
        }
        if(player_col == 2 && empty_col ==1)
        {
            x1++;
        }
        // Check x2 rows and columns
        if(oppo_row == 2 && empty_row ==1)
        {
            x2++;
        }
        if(oppo_col ==2 && empty_col ==1)
        {
            x2++;
        }
        // Check x5 rows and columns
        if(player_row == 1 && empty_row == 2)
        {
            x5++;
        }
        if(player_col == 1 && empty_col == 2)
        {
            x5++;
        }
        // Check x6 rows and columns
        if(player_row ==3)
        {
            x6++;
        }
        if(player_col == 3)
        {
            x6++;
        }
    }
    }
    // Check diagonals
    for(int i =0; i<2;i++)
    {
        int player_dia =0, empty_dia=0, oppo_dia=0,temp=0;
        for(int j=0; j<3;j++)
        {
            // Set temporary value from the board
            if(i == 0)
            {
                // Bottom-left to top-right diagonal
                temp = CBoard[2-j][j];
            }
            else
            {
                // Top-left to bottom-right diagonal
                temp = CBoard[j][j];
            }
            // Check diagonal counts
            if(temp == Cplayer)
            {
                player_dia++;
            }
            else if(temp == 0)
            {
                empty_dia++;
            }
            else
            {
                oppo_dia++;
            }
        }
        // Check x1
        if(player_dia == 2&& empty_dia == 1)
        {
            x1++;
        }
        // Check x2
        if(oppo_dia ==2 && empty_dia ==1)
        {
            x2++;
        }
        // Check x5
        if(player_dia ==1 && empty_dia==2)
        {
            x5++;
        }
        // Check x6
        if(player_dia ==3)
        {
            x6++;
        }
    }
    // Set features based on the referenced snippet
    board_feature[0] = x0;
    board_feature[1] = x1;
    board_feature[2] = x2;
    board_feature[3] = x3;
    board_feature[4] = x4;
    board_feature[5] = x5;
    board_feature[6] = x6;
}

// Evaluate and assign value to board state
float Eval_Approx(int features[FEATURES], float weight[FEATURES])
{
    float value =0;
    for(int i=0; i<FEATURES;i++)
    {
        value += features[i] * weight[i];
    }
    return value;
}

// Update weight to be used for ML
void updateWeight(float LR, int features[FEATURES], float weights[FEATURES],float y, float yHat)
{
    for(int i = 0; i < FEATURES; i++)
    {
        weights[i] = weights[i] + LR * (y-yHat) * features[i]; // LR is the learning rate; y is the result and yHat is the prediction
    }
}

// Unified function to process model data; control updates and writes via flags
void processModelData (const char* phase, int dataset[][BOARD + 1], int size, float weight[FEATURES], int updateWeights, int writeWeights)
{
    int Com_player = ML_X;
    float Yhat = -1, y = 0, MMSE = 0;

    // Error counting
    int total_errors = 0;
    int *errors = (int*)calloc(size, sizeof(int)); // Array to store individual errors

    // Confusion matrix metrics
    int TP = 0, TN = 0, FP = 0, FN = 0;

    for(int i = 0;i < size; i++)
    {
        get_board_features(dataset[i],Com_player);
        Yhat = Eval_Approx(board_feature,weight);
        y = dataset[i][9];
        
        MMSE += pow(y - Yhat, 2);                                               // Calculate Minimum Mean Square Error
        if (updateWeights)
        {
            updateWeight(learning_rate, board_feature, weight, y, Yhat);        // Update ML weight (training only)
        }
        evaluateModel(Yhat, y, &TP, &TN, &FP, &FN, &total_errors, errors, i);   // Evaluate model metrics
    }

    // Calculate metrics
    MMSE /= size;  // Divide by dataset size to get average

    // Write weights if requested (e.g., after training)
    if (writeWeights)
    {
        writeweights();
    }
    
    // Print results
    printModelMetrics(phase, MMSE, total_errors, size, TP, TN, FP, FN);
    free(errors);   // Free allocated memory
}

// Function to evaluate model metrics
void evaluateModel(float Yhat, float y, int* TP, int* TN, int* FP, int* FN, int* total_errors, int* errors, int index) 
{
    // Convert continuous predictions to discrete classifications (-1, 0, 1)
    int predicted_class;
    if (Yhat > CLASS_THRESHOLD) 
    {
        predicted_class = 1;    // Win prediction
    } 
    else if (Yhat < -CLASS_THRESHOLD) 
    {
        predicted_class = -1;   // Loss prediction
    } 
    else 
    {
        predicted_class = 0;    // Draw prediction
    }

    // Record error (1 if prediction is wrong, 0 if correct)
    errors[index] = (predicted_class != (int)y) ? 1 : 0;
    *total_errors += errors[index];
    
    // Update confusion matrix metrics (focusing on win/loss classification)
    if (y == 1) 
    { // Actual positive (win)
        if (predicted_class == 1)
        {
            (*TP)++;
        } 
        else if (predicted_class == -1)
        {
            (*FN)++;
        } 
    } 
    else if (y == -1) 
    { // Actual negative (loss)
        if (predicted_class == -1) 
        {
            (*TN)++;
        }
        else if (predicted_class == 1) 
        {
            (*FP)++;
        }
    }
}

// Function to print model metrics
void printModelMetrics(const char* phase, float MMSE, int total_errors, int size, int TP, int TN, int FP, int FN) 
{
    float error_probability = (float)total_errors / size;
    float accuracy = ((float)TP + TN) / size;
    float precision = (float)TP / (TP + FP);
    float recall = (float)TP / (TP + FN);
    float f_score = 2 * (precision * recall) / (precision + recall);

    printf("\n=== Model %s Evaluation Results ===\n", phase);
    printf("Minimum Mean Squared Error: %f\n", MMSE);
    printf("Probability of Error: %f\n", error_probability);
    printf("\nClassification Metrics:\n");
    printf("Accuracy: %f\n", accuracy);
    printf("Precision: %f\n", precision);
    printf("Recall: %f\n", recall);
    printf("F-score: %f\n", f_score);

    printf("\nConfusion Matrix Metrics:\n");
    printf("True Positives (correct win predictions): %d\n", TP);
    printf("True Negatives (correct loss predictions): %d\n", TN);
    printf("False Positives (incorrect win predictions): %d\n", FP);
    printf("False Negatives (incorrect loss predictions): %d\n", FN);
}

// Train model base on training dataset & evaluate
void trainModel(int training_set[TRAINING_SIZE][BOARD +1],float weight[FEATURES])
{
    // Epoch-based training: shuffle and update weights over multiple passes,
    // then evaluate and write weights once at the end to avoid excessive logging.
    int Com_player = ML_X;
    for (int epoch = 0; epoch < EPOCH; epoch++)
    {
        // Optional: shuffle per-epoch for better generalization
        shuffle_rows(training_set, TRAINING_SIZE);

        // Single pass weight updates across the training set
        for (int i = 0; i < TRAINING_SIZE; i++)
        {
            // Compute features and prediction
            get_board_features(training_set[i], Com_player);
            float Yhat = Eval_Approx(board_feature, weight);
            float y = training_set[i][9];

            // Gradient update
            updateWeight(learning_rate, board_feature, weight, y, Yhat);
        }
    }

    // Final evaluation and write-out (no further updates during evaluation)
    processModelData("Training", training_set, TRAINING_SIZE, weight, 0, 1);
}

// Test model using training weights & evaluate
void testModel(int testing_set[TESTING_SIZE][BOARD +1],float weight[FEATURES])
{
    processModelData("Testing", testing_set, TESTING_SIZE, weight, 0, 0);
}

// Get the best move the AI can make using the trained weights
void BestMove(int state[BOARD],float weights[FEATURES],int player)
{
    int moveC = 0, best_move= 0;
    float val = 0.0f;
    float best_val = (player == PLAYER_X) ? -FLT_MAX : FLT_MAX;
    
    //Set board to 0 and label to -1
    for (int i = 0; i < BOARD; i++) {
        for (int j = 0; j < BOARD + 1; j++) {
            if (j == 9) 
            {
                moves[i][9] = -1;
            }
            else 
            {
                // Reset
                moves[i][j] = 0;
            }
        }
    }

    //set all possibles moves
    for(int i=0; i<BOARD; i++)
    {
        if(state[i] == 0)
        {
            for(int j = 0; j<BOARD; j++)
            {
                moves[moveC][j] = state[j];
            }
            moves[moveC][i] = player;
            moves[moveC][9] = i;
            moveC++;
        }
    }

    //Iterate through each possible move
    for(int i =0;i<BOARD;i++)
    {
        if(moves[i][9] != -1)
        {
            // Always evaluate features from X's perspective
            get_board_features(moves[i], ML_X);
            val = Eval_Approx(board_feature, weights);
            // Maximize if current player is X; minimize if O
            if ((player == PLAYER_X && val > best_val) || (player == PLAYER_O && val < best_val))
            {
                best_val = val;
                best_move = moves[i][9];
            }
        }
        else
        {
            break;
        }
    }
    // Set best move to board
    board_state[best_move] = player;
    //printBoard(board_state);
}

// Print board to CLI
void printBoard(int state[BOARD])
{
    printf("  %2d  |  %2d  |  %2d\n",state[0],state[1],state[2]);
    printf("------+------ +------\n");
    printf("  %2d  |  %2d  |  %2d\n",state[3],state[4],state[5]);
    printf("------+------ +------\n");
    printf("  %2d  |  %2d  |  %2d\n\n",state[6],state[7],state[8]);
}

// Reset the board for the next game
void reset()
{
    for (int i = 0; i < BOARD; i++) 
    {
        board_state[i] = 0;
    }
}

// Get player input
void playerInput(int playerNo)
{
    int player_move = 0;
    do {
        printf("\nSelect the position (0 to 8) to place your piece: ");
        scanf("%d", &player_move);

        // Check if move is legal
        if (player_move < 0 || player_move >= BOARD) {
            printf("\nIllegal move!");
            continue;
        }
        // Check if current cell selected is occupied
        else if (board_state[player_move] != 0) { 
            printf("\nPosition %d is already occupied!", player_move);
            continue;
        }
        // Set current move
        else {
            board_state[player_move] = playerNo;
            printBoard(board_state);
            break;
        }
    } while (1);
}

// Player vs ML
void playAgainstML()
{

    int board_status =9,game_no = 1, current_player=0,turn=0,MLWins=0,PlayerWins=0,draws=0;
    char play_again;
    
    // Start a while loop to keep the game running until the user decides not to play again
    do {        
        // Clear and set state
        reset();
        board_status = 9;

        // Alternate the starting player
        if (game_no % 2 == 0) {
            current_player = 1;
            turn = 0;
        } else {
            current_player = 2;
            turn = 1;
        }

        for (int i = 0; i < SIZE; i++) {
            // ML first
            if (turn == 0) {
                printf("ML turn\n");
                BestMove(board_state, init_weight, current_player);
                // Get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if (board_status != 9) {
                    break;
                }
                current_player = (current_player == 1) ? 2 : 1;

                // Player turn
                printf("Your turn\n");
                printBoard(board_state);
                playerInput(current_player);
                // Get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if (board_status != 9) {
                    break;
                }
                current_player = (current_player == 1) ? 2 : 1;
            }
            // Player first
            else {
                // Player turn
                printf("Your turn\n");
                printBoard(board_state);
                playerInput(current_player);
                board_status = BoardState(board_state);
                printBoard(board_state);
                if (board_status != 9) {
                    break;
                }
                current_player = (current_player == 1) ? 2 : 1;

                // ML
                printf("ML turn\n");
                BestMove(board_state, init_weight, current_player);
                // Get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if (board_status != 9) {
                    break;
                }
                current_player = (current_player == 1) ? 2 : 1;
            }
        }

        // Determine winner or draw
        if (board_status == 0) {
            printBoard(board_state);
            printf("\nDRAW!\n");
            draws++;
        } else if (board_status == 1) {
            printBoard(board_state);
            printf("\nML WINS!\n");
            MLWins++;
        } else {
            printBoard(board_state);
            printf("\nYou WIN!\n");
            PlayerWins++;
        }
    

        // Display the score after each game session
        printf("\nScores:\n");
        printf("ML Wins: %d\n", MLWins);
        printf("Your Wins: %d\n", PlayerWins);
        printf("Draws: %d\n", draws);
        //Increment game_no so that it alternates between player and AI
        game_no++;

        // Prompt the user to play again
        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &play_again); 

    } while (play_again == 'y' || play_again == 'Y');
}

// Evaluate ML vs Minimax algorithm
void MLvsMinMAx(int length)
{
    // 1 is ML 2 is MinMax
    int board_status =9, current_player=0,turn=0,MLWins=0,MMXwins=0,draws=0;
    for(int game_no = 1;game_no <= length;game_no++)
    {
        //clear and set state
        reset();
        board_status=9;

        // Alternate the starting player
        if(game_no % 2 ==0)
        {
            current_player = 1;
            turn =0;
        }
        else{
            current_player =2;
            turn= 1;
        }

        for(int i =0;i < SIZE;i++)
        {
            //ML first
            if(turn == 0)
            {
                printf("ML turn\n");
                BestMove(board_state,init_weight,current_player);
                //get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if(board_status != 9)
                {
                    break;
                }
                if(current_player == 1)
                {
                    current_player = 2;
                }
                else
                {
                    current_player =1;
                }
                // Minimax move
                printf("MMX turn\n");
                board_state[MMXBest(board_state,current_player)] = current_player;
                //get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if(board_status != 9)
                {
                    break;
                }
                if(current_player == 1)
                {
                    current_player = 2;
                }
                else
                {
                    current_player =1;
                }
            }
            //MinMax first
            else
            {
                
                //MinMax
                printf("MMX turn\n");
                board_state[MMXBest(board_state,current_player)] = current_player;
                board_status = BoardState(board_state);
                printBoard(board_state);
                if(board_status != 9)
                {
                    break;
                }
                if(current_player == 1)
                {
                    current_player = 2;
                }
                else
                {
                    current_player =1;
                }
                //ML
                printf("ML turn\n");
                BestMove(board_state,init_weight,current_player);
                //get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if(board_status != 9)
                {
                    break;
                }
                if(current_player == 1)
                {
                    current_player = 2;
                }
                else
                {
                    current_player =1;
                }
            }

        }
        // Print result and increment the corresponding counter
        if(board_status == 0)
        {
            printBoard(board_state);
            printf("\nDRAW!\n");
            draws++;
        }
        else if(board_status == 1)
        {
            printBoard(board_state);
            printf("\nML WINS!\n");
            MLWins++;
        }
        else
        {
            printBoard(board_state);
            printf("\nMINMAX WINS!\n");
            MMXwins++;
        }

}
    printf("ML: %d MinMax:%d Draws:%d",MLWins,MMXwins,draws);
}

int MMXBest(int state[BOARD],int CurrentPlayer)
{
    //changing a 1d array to 2d
    int arr[3][3];
    arr[0][0] = state[0];
    arr[0][1] = state[1];
    arr[0][2] = state[2];
    arr[1][0] = state[3];
    arr[1][1] = state[4];
    arr[1][2] = state[5];
    arr[2][0] = state[6];
    arr[2][1] = state[7];
    arr[2][2] = state[8];

    int bestVal = INT_MIN;
    int bestRow = -1, bestCol = -1;

    // Recursively trigger minimax for all the moves
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (arr[i][j] == 0) {
                arr[i][j] = CurrentPlayer; //Place Comp move
                int moveVal = minimax(arr, 0, 0,CurrentPlayer);
                arr[i][j] = 0; // Clear moves

                //Evaluate if the move is better or not
                if (moveVal > bestVal) {
                    bestRow = i;
                    bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return (bestRow * 3 + bestCol); // Converts 2D array indices into 1D
    
}



int minimax(int board[3][3], int depth, int isMaximizingPlayer,int CurrentPlayer) {
    int score = evaluate(board,CurrentPlayer);

    // Using 10/-10 for win/loss works with depth adjustments; using ±1 makes tuning via depth more difficult
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    // Stop if there are no moves left or if depth reaches the limit
    if (!isMovesLeft(board) || depth == 9) return 0;
    // Minimax aims to maximize the current player's score while minimizing the opponent's
    // Minimax needs to identify the current player
    if (isMaximizingPlayer) {
        int best = INT_MIN; //Ai starts with the lowest score
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = CurrentPlayer; //Ai move
                    //Checking if there is a higher score then store it within best if it is higher then the current "best" value.
                    //if(minimax(board, depth + 1, 0) > best) {best = minimax(board, depth + 1, 0);} AI. 1 = AI, 0 = Player
                    best = (minimax(board, depth + 1, 0,CurrentPlayer) > best) ? minimax(board, depth + 1, 0,CurrentPlayer) : best;
                    board[i][j] = 0; //Clear the move for next recursion
                }
            }
        }
        return best; //The highest possible score within 10
    } else {
        int best = INT_MAX; //Player starts with the highest score
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = CurrentPlayer - 1; //Player simulated move
                    //Checking if there is a lowest score then store it within best
                    best = (minimax(board, depth + 1, 1,CurrentPlayer) < best) ? minimax(board, depth + 1, 1,CurrentPlayer) : best;
                    board[i][j] = 0; //Clear the move for next recursion
                }
            }
        }
        return best; //The closest possible score to -10
    }
}

// Evaluates the current board state and returns a score
int evaluate(int board[3][3],int CurrentPlayer) {
   //Checking rows and column for win
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == CurrentPlayer) return 10;
            if (board[i][0] == CurrentPlayer -1) return -10;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == CurrentPlayer) return 10;
            if (board[0][i] == CurrentPlayer -1) return -10;
        }
    }
    // Checking diagonals for win
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == CurrentPlayer) return 10;
        if (board[0][0] == CurrentPlayer -1) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == CurrentPlayer) return 10;
        if (board[0][2] == CurrentPlayer -1) return -10;
    }
    return 0; //return draw
}

// Checks if there are any empty cells left on the board
int isMovesLeft(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) return 1;
        }
    }
    return 0;
}

// Selects a random legal move
void randomInput(int state[BOARD], int player)
{
    int randomMove;
    do
    {
        randomMove = rand() % 9; // select number between 0 and 8
    } while (state[randomMove] != 0); // if space is  occupied, continue the loop
    
    //set move
    state[randomMove] = player;
}

// Evaluate ML vs random inputs
void MLvsRandom(int length)
{
    // 1 is ML 2 is random
    int board_status =9, current_player=0,turn=0,MLWins=0,Randomwins=0,draws=0;
    for(int game_no = 1;game_no <= length;game_no++)
    {
        //clear and set state
        reset();
        board_status=9;

        //alternate between starting player
        if(game_no % 2 ==0)
        {
            current_player = 1;
            turn =0;
        }
        else{
            current_player =2;
            turn= 1;
        }

        for(int i =0;i < SIZE;i++)
        {
            //ML first
            if(turn == 0)
            {
                printf("ML turn\n");
                BestMove(board_state,init_weight,current_player);
                //get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if(board_status != 9)
                {
                    break;
                }
                if(current_player == 1)
                {
                    current_player = 2;
                }
                else
                {
                    current_player =1;
                }
                //Random input
                printf("Random's turn\n");
                randomInput(board_state,current_player);
                //get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if(board_status != 9)
                {
                    break;
                }
                if(current_player == 1)
                {
                    current_player = 2;
                }
                else
                {
                    current_player =1;
                }
            }
            //Random first
            else
            {
                
                //Random input
                printf("Random's turn\n");
                randomInput(board_state,current_player);
                board_status = BoardState(board_state);
                printBoard(board_state);
                if(board_status != 9)
                {
                    break;
                }
                if(current_player == 1)
                {
                    current_player = 2;
                }
                else
                {
                    current_player =1;
                }
                //ML
                printf("ML turn\n");
                BestMove(board_state,init_weight,current_player);
                //get board status
                board_status = BoardState(board_state);
                printBoard(board_state);
                if(board_status != 9)
                {
                    break;
                }
                if(current_player == 1)
                {
                    current_player = 2;
                }
                else
                {
                    current_player =1;
                }
            }

        }
        //print result of the games and add wins to counter
        if(board_status == 0)
        {
            printBoard(board_state);
            printf("\nDRAW!\n");
            draws++;
        }
        else if(board_status == 1)
        {
            printBoard(board_state);
            printf("\nML WINS!\n");
            MLWins++;
        }
        else
        {
            printBoard(board_state);
            printf("\nRandom WINS!\n");
            Randomwins++;
        }

}
    printf("ML: %d Random:%d Draws:%d",MLWins,Randomwins,draws);
}

//write weights to a txt file
void writeweights()
{
    //Open file for writing
    FILE *writeOut;
    writeOut = fopen("models/MLWeights.txt","w");
    if(writeOut == NULL)
    {
        printf("File does not exist\n");
        exit(1);
    }

    for(int i=0; i<FEATURES;i++)
    {
        fprintf(writeOut,"%f\n",init_weight[i]);
    }
    fclose(writeOut);
}

void TrainwithItself(int cycles, float weight[FEATURES])
{
    // 1 is controlled ML 2 is opponents
    int board_status =9, current_player=0,controlled_player=1,result=0;
    float actual_value,predicted_value,outcome;

    int temp_features[FEATURES]; //temp features to be used by controlled ML

    //pit ml vs ml for no of cycle
    for(int i=0; i<cycles;i++)
    {
        reset(); // reset board after each game
        board_status =9;
        //change who starts first
        if(i % 2 == 0)
        {
            current_player =1;
        }
        else
        {
            current_player =2;
        }
        for(int j=0;j<SIZE;j++)
        {
            get_board_features(board_state,controlled_player);
            predicted_value = Eval_Approx(board_feature,init_weight);
            //set current weight into temp
            for(int k=0;k<FEATURES;k++)
            {
                temp_features[k] = board_feature[k];
            }
            
            //Controlled ML Move
            BestMove(board_state,init_weight,current_player);
            board_status = BoardState(board_state);
            if(board_status != 9)
            {
                break;
            }
            //opposing ML Move
            if(current_player == 1)
            {
                current_player =2;
            }
            else
            {
                current_player =1;
            }
            BestMove(board_state,init_weight,current_player);
            board_status = BoardState(board_state);
            if(board_status != 9)
            {
                break;
            }
            if(current_player == 1)
            {
                current_player =2;
            }
            else
            {
                current_player =1;
            }
            //get feature and actual value
            get_board_features(board_state,controlled_player);
            actual_value = Eval_Approx(board_feature,init_weight);
            updateWeight(learning_rate,temp_features,init_weight,actual_value,predicted_value);
        }

        //set result after a game is finished
        if(board_status == 0)
        {
            result =0;
        }
        else if(board_status == 1)
        {
            result = 1;
        }
        else
        {
            result = -1;
        }
        //get feature and outcome value
        get_board_features(board_state,controlled_player);
        outcome = Eval_Approx(board_feature,init_weight);
        updateWeight(learning_rate,temp_features,init_weight,result,outcome);

}
reset();
}
