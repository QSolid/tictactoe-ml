/*Prototypes and Global Variables that are being used in ML.c*/
#define BOARD 9                                     // Grids of tic-tac-toe board
#define FEATURES 7                                  // Number of features 
#define DATA_FILE "training/data/tic-tac-toe.data"  // File path for dataset (relative to repo root)
#define SIZE 958                                    // Total lines in dataset
#define TRAINING_SIZE 764                           // 80% of total manual calc might overflow
#define TESTING_SIZE 194                            // 20% of total manual calc might overflow
#define CLASS_THRESHOLD 0                           // Classification threshold
#define GAME_LENGTH 1000                            // How many games to play
#define EPOCH 1
// Dataset/player markers for ML feature evaluation
#define ML_EMPTY 0
#define ML_X 1
#define ML_O -1
// CLI game player ids used in training/evaluation loops
#define PLAYER_X 1
#define PLAYER_O 2


// ML Prototypes
void extract(char*, int a[SIZE][BOARD + 1]);     // extract file that contains the data that is to be used
int convert_input(const char *input);            // convert input of the data file into machine usable format
int BoardState(int state[BOARD]);                // check if board is running 
void cat_data();                                 // categorise data into wins lose and draw
void split_data();                               // split data into training and testing array without reduplicating the same data point
// Feature extraction and evaluation are provided via ../src/ml_shared.h
void updateWeight(float LR, int features[FEATURES], float weights[FEATURES],float y, float yHat); // Update weight to be used for ML
void writeweights();
void trainModel(int training_set[TRAINING_SIZE][BOARD +1],float weight[FEATURES]); // Train model base on training dataset & evaluat
void testModel(int testing_set[TESTING_SIZE][BOARD +1],float weight[FEATURES]); // Test model using training weights & evaluate
void BestMove(int state[BOARD],float weights[FEATURES],int player); // Get the best move the AI can make using the trained weights
void evaluateModel(float Yhat, float y, int* TP, int* TN, int* FP, int* FN, int* total_errors, int* errors, int index); // Function to evaluate model metrics
void printModelMetrics(const char* phase, float MMSE, int total_errors, int size, int TP, int TN, int FP, int FN); // Function to print model metrics
void processModelData(const char* phase, int dataset[][BOARD + 1], int size, float weight[FEATURES], int updateWeights, int writeWeights); // Unified function to process model data
void TrainwithItself(int cycles, float weight[FEATURES]);
void shuffle_rows(int a[][BOARD + 1], int count);


// Game Prototypes
void printBoard(int state[BOARD]); // Print board to CLI
void reset(); // Reset board for another game
void playerInput(int playerNo); // get player's input
void playAgainstML(); // Player vs ML
void MLvsMiniMax(int length); // Evaluate between ML and MiniMax Algorithm
void randomInput(int state[BOARD], int player); // Randomly comes up with moves 
void MLvsRandom(int length); // Evaluate between ML and Random input

//MiniMax Prototypes
int MMXBest(int state[BOARD],int CurrentPlayer); // Get the best move MiniMax Algo can make
int minimax(int board[3][3], int depth, int isMaximizingPlayer,int CurrentPlayer); // Recursive function to get best move
int evaluate(int board[3][3],int CurrentPlayer); // Evaluates the current board state and returns a score.
int isMovesLeft(int board[3][3]); // Checks if there are any empty cells left on the board.

// Global variables
int dataset[SIZE][BOARD + 1]; // Full dataset
int win_data[SIZE][BOARD + 1]; // Only wins
int loss_data[SIZE][BOARD + 1]; // Only loss
int draw_data[SIZE][BOARD+1]; // Only draw
int training_dataset[TRAINING_SIZE][BOARD+1]; // Dataset that is going to be use for training AI
int testing_dataset[TESTING_SIZE][BOARD + 1]; // Dataset that is going to be use for testing AI
int legal_wins[8][3] ={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}}; // Legal win conditions
int board_feature[FEATURES]; // Array for feature
int board_state[BOARD];
float init_weight[FEATURES] ={1.0,1.0,1.0,1.0,1.0,1.0,1.0}; // Starting weights
int moves[BOARD][BOARD +1]; // For moves that are still available 
int wins=0,lose=0,draw=0; // Total number of each
float learning_rate = 0.01; // Speed of the ML 
