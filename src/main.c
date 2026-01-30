#include <gtk/gtk.h>
#include <stdlib.h>
#include <limits.h> //for MINMAX ALGO
#include "minimax.h"
#include "ML-config.h"
#include "game_logic.h"
#include <windows.h>


#define PLAYER 1   // for MINMAX ALGO
#define COMPUTER 2 // for MINMAX ALGO
#define EMPTY 0    // for MINMAX ALGO

void compMove();
// Prototyping for ML Integration
void makeMLMove();

// Declaring global variable for button and label
GtkWidget *button11;
GtkWidget *button12;
GtkWidget *button13;
GtkWidget *button21;
GtkWidget *button22;
GtkWidget *button23;
GtkWidget *button31;
GtkWidget *button32;
GtkWidget *button33;
GtkWidget *modebutton;
GtkLabel *gamemode;
GtkLabel *game_stats;
GtkLabel *statusbtn;

// declaring global variables
GtkButton *diffClick, *gameClick;
GtkButton *button[3][3] = {{NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}};

// declaring global flags
int flag = 0, gameNotOver = 1, initialise = 0, statusFlag = 0, pressed[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int moveCounter = 0, total_game_played = 0, player_1_win = 0, player_2_win = 0, game_draw = 0;

// declaring game parameters
int arr[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int gameType = 0, gameDifficulty = 1;

// main method, start of execution
int main(int argc, char *argv[])
{
  //// Declare pointers for GTKBuilder and the main window widget
  GtkBuilder *builder;
  GtkWidget *start;

  gtk_init(&argc, &argv);
  init_ml_system(); // Initialise ML system

  // Initializing GTKbuilder with .glade file
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "assets/tictactoeUI.glade", NULL);

  // connect to css file
  start = GTK_WIDGET(gtk_builder_get_object(builder, "startWindow"));
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "assets/style.css", NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);

  g_object_unref(provider); // release the css resource

  // connect to the signal handlers definede in the glade file
  gtk_builder_connect_signals(builder, NULL);

  // Initializing main window widget with "startWindow" id from glade file (start page)
  start = GTK_WIDGET(gtk_builder_get_object(builder, "startWindow"));

  // release gtk builder resource by dereferencing the builder pointer
  g_object_unref(builder);

  // Showing main window widget
  gtk_widget_show(start);

  // Open GTK main loop (waiting for user button presses)
  gtk_main();

  return 0; // exit
}



// Main ML move function
void makeMLMove() 
{
  if (!gameNotOver) return; // Return if game is already over to prevent additional moves
    
  // Convert and copy board state
  int mlBoard[BOARD]; // Create temporary 1D array to convert GUI's 2D board state (arr[3][3])
  int idx = 0;        // into ML's expected 1D format (array of 9 elements)

  // Convert GUI board state (which uses 0=empty, 1=X, 2=O)
  // to ML board state (which uses 0=empty, 1=X, -1=O)
  for(int i = 0; i < 3; i++) 
  {
    for(int j = 0; j < 3; j++) 
    {
      if(arr[i][j] == 1) 
      {
        mlBoard[idx] = 1;   // Player X stays as 1
      } else if(arr[i][j] == 2) 
      {
        mlBoard[idx] = -1;  // Player O converts from 2 to -1
      } else 
      {
        mlBoard[idx] = 0;   // Empty cells stay as 0
      }
      idx++;
    }
  }

  // Copy the converted board state to ML's global board state array
  // This is necessary because BestMove() works with the global ml_board_state  
  for(int i = 0; i < BOARD; i++) 
  {
    ml_board_state[i] = mlBoard[i];
  }
    
    // Get ML's move for computer O perspective
    BestMove(ml_board_state, init_weight, -1);
    
  // Find move position and update GUI
  for(int i = 0; i < BOARD; i++) 
  {
    if(ml_board_state[i] != mlBoard[i]) 
    {
      // Convert 1D position back to 2D coordinates for GUI board
      int row = i / 3;  // Integer division by 3 gives row number
      int col = i % 3;  // Remainder gives column number
      arr[row][col] = 2;  // Update GUI board state with computer's move (O is represented as 2)

      // Create array of GUI button pointers for easy access
      GtkButton *buttons[3][3] = {
      {GTK_BUTTON(button11), GTK_BUTTON(button12), GTK_BUTTON(button13)},
      {GTK_BUTTON(button21), GTK_BUTTON(button22), GTK_BUTTON(button23)},
      {GTK_BUTTON(button31), GTK_BUTTON(button32), GTK_BUTTON(button33)}
      };
      // Set button label to "O" 
      gtk_button_set_label(buttons[row][col], "O");
      gtk_widget_set_name(GTK_WIDGET(buttons[row][col]), "player_2");
      // Mark position as occupied and increment move counter
      pressed[row][col] = 1;
      moveCounter++;
      // Check if game has ended (win or draw)
      int winner = anywinner(arr);
      if (winner || moveCounter == 9) 
      {
        if (winner == 0 && moveCounter == 9) // All cells filled with no winner = draw
        {
          update_game_status("DRAW", 0);
        } else if (winner == 2) 
        {
          update_game_status("COMPUTER WON", 2);
          play_sound("lose.wav");
        }
        return;
      }
      // Game continues - update status to indicate next player's turn
      update_game_status("PLAYER 2'S MOVE", 4);
      break;  // Exit loop after processing the move
    }
  }
}






// end of program
