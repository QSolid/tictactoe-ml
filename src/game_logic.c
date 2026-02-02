#include "game_logic.h"
#include <stdio.h>

void on_pvpstartbutton_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Declare pointers for GTKBuilder and main window widget
  GtkBuilder *builder;
  GtkWidget *window;

  // Initialise GTKBuilder with the .glade file
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "assets/tictactoeUI.glade", NULL);

  // Initialise main window widget and buttons
  window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
  button11 = GTK_WIDGET(gtk_builder_get_object(builder, "button11"));
  button12 = GTK_WIDGET(gtk_builder_get_object(builder, "button12"));
  button13 = GTK_WIDGET(gtk_builder_get_object(builder, "button13"));
  button21 = GTK_WIDGET(gtk_builder_get_object(builder, "button21"));
  button22 = GTK_WIDGET(gtk_builder_get_object(builder, "button22"));
  button23 = GTK_WIDGET(gtk_builder_get_object(builder, "button23"));
  button31 = GTK_WIDGET(gtk_builder_get_object(builder, "button31"));
  button32 = GTK_WIDGET(gtk_builder_get_object(builder, "button32"));
  button33 = GTK_WIDGET(gtk_builder_get_object(builder, "button33"));
  // Initialise labels for game mode, game stats, and status
  gamemode = GTK_LABEL(gtk_builder_get_object(builder, "gamemode"));
  game_stats = GTK_LABEL(gtk_builder_get_object(builder, "game_stats"));
  statusbtn = GTK_LABEL(gtk_builder_get_object(builder, "statusbtn"));

  // Connect to all signals defined in the glade file
  gtk_builder_connect_signals(builder, NULL);

  // Release GTK builder resources by dereferencing the builder pointer
  g_object_unref(builder);

  // Destroy the current widget after Player vs Player button is clicked
  gtk_widget_destroy((GtkWidget *)user_data);

  // Show main window widget (game page)
  gtk_widget_show(window);

  // Set initial game variables to Player vs Player
  gameType = 0;   // set game type to 0 (player vs player)
  statusFlag = 1; // Set status flag to 1 (start game)
  flag = 1;

  update_game_status("PLAYER 1'S MOVE", 4);

  // Set label for game mode to not available (no mode for Player vs Player)
  gtk_label_set_text(gamemode, "Game mode not available");
}

void on_pvcstartbutton_clicked(GtkWidget *click_button, gpointer user_data)
{
  GtkBuilder *builder;
  GtkWidget *window;

  // Initialise GTKBuilder with the .glade file
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "assets/tictactoeUI.glade", NULL);

  // Initialise main window, buttons, and labels for game mode and status
  window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
  button11 = GTK_WIDGET(gtk_builder_get_object(builder, "button11"));
  button12 = GTK_WIDGET(gtk_builder_get_object(builder, "button12"));
  button13 = GTK_WIDGET(gtk_builder_get_object(builder, "button13"));
  button21 = GTK_WIDGET(gtk_builder_get_object(builder, "button21"));
  button22 = GTK_WIDGET(gtk_builder_get_object(builder, "button22"));
  button23 = GTK_WIDGET(gtk_builder_get_object(builder, "button23"));
  button31 = GTK_WIDGET(gtk_builder_get_object(builder, "button31"));
  button32 = GTK_WIDGET(gtk_builder_get_object(builder, "button32"));
  button33 = GTK_WIDGET(gtk_builder_get_object(builder, "button33"));
  gamemode = GTK_LABEL(gtk_builder_get_object(builder, "gamemode"));
  statusbtn = GTK_LABEL(gtk_builder_get_object(builder, "statusbtn"));
  game_stats = GTK_LABEL(gtk_builder_get_object(builder, "game_stats"));
  modebutton = GTK_WIDGET(gtk_builder_get_object(builder, "chooseDifficultyButton"));
  gtk_builder_connect_signals(builder, NULL);

  // Release GTK builder resources
  g_object_unref(builder);

  gtk_widget_destroy((GtkWidget *)user_data);
  // Show main window widget
  gtk_widget_show(window);
  // Set initial game variables
  gameType = 1;   // set game type to 1 (player vs computer)
  statusFlag = 1; // Set status flag to 1 (indicates game start)
  flag = 1;
  gameDifficulty = 1; // Set game mode difficulty to 1 (Medium mode at start)

  update_game_status("PLAYER 1'S MOVE", 4);
  // Set label for game mode
  gtk_label_set_text(gamemode, "Medium");
}

// Run when Main Menu button on the game page is clicked; returns to the start menu
void on_mainmenu_clicked(GtkWidget *click_button, gpointer user_data)
{
  GtkBuilder *builder;
  GtkWidget *start;

  // Initialise GTKBuilder with the .glade file
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "assets/tictactoeUI.glade", NULL);

  // Initialise main window widget
  start = GTK_WIDGET(gtk_builder_get_object(builder, "startWindow"));

  gtk_builder_connect_signals(builder, NULL);

  // Release GTK builder resources by dereferencing the builder pointer
  g_object_unref(builder);

  // Reset board and game
  resetgame();
  player_1_win = player_2_win = game_draw = 0;

  // Destroy current window
  gtk_widget_destroy((GtkWidget *)user_data);

  // Show start menu
  gtk_widget_show(start);
}

// Called when Quit is selected in the Settings menu
void on_gameExit_activate()
{
  gtk_main_quit();
}

// Run when game mode left button is clicked (decrease game mode)
void on_clickleft_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Only run when game mode is 1 (Player vs Computer)
  if (gameType == 1)
  {
    // set game difficulty to only 0 1 2 (easy , medium, hard) in a decreasing format
    gameDifficulty = (gameDifficulty - 1 + 4) % 4;

    // reset game and wins when game mode is change
    resetgame();
    player_1_win = player_2_win = game_draw = 0;

    // change label button base on difficulty
    switch (gameDifficulty)
    {
    case 0:
      gtk_label_set_text(gamemode, "Easy");
      break;
    case 1:
      gtk_label_set_text(gamemode, "Medium");
      break;
    case 2:
      gtk_label_set_text(gamemode, "Hard");
      break;
    case 3:
      gtk_label_set_text(gamemode, "ML");
      break;

    default:
      gtk_label_set_text(gamemode, "Broken");
      break;
    }
  }
}

// Run when game mode right button is clicked (increase game mode)
void on_clickright_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Only run when game mode is 1 (Player vs Computer)
  if (gameType == 1)
  {
    // Set game difficulty to only 0 1 2 (easy , medium, hard, ML) in an increasing format
    gameDifficulty = (gameDifficulty + 1) % 4;

    // Reset game and wins when game mode is change
    resetgame();
    player_1_win = player_2_win = game_draw = 0;

    // Change label button base on difficulty
    switch (gameDifficulty)
    {
    case 0:
      gtk_label_set_text(gamemode, "Easy");
      break;
    case 1:
      gtk_label_set_text(gamemode, "Medium");
      break;
    case 2:
      gtk_label_set_text(gamemode, "Hard");
      break;
    case 3:
      gtk_label_set_text(gamemode, "ML");
      break;

    default:
      gtk_label_set_text(gamemode, "Broken");
      break;
    }
  }
}

// Run when Restart is selected
void on_restartGame_activate(GtkWidget *click_button, gpointer user_data)
{
  // If game not started, no point resetting
  if (!statusFlag)
    return;

  // Call reset game function
  resetgame();

  // Set label
  gtk_label_set_text(statusbtn, "Game restarted. Player 1 make your move!");
}

void update_game_status(const char *status_text, int winner)
{
  char stats[100];
  gtk_label_set_text(statusbtn, status_text);
  if (winner == 1)
  {
    player_1_win++;
  }
  else if (winner == 2)
  {
    player_2_win++;
  }
  else if (winner == 0)
  {
    game_draw++;
  }
  if(gameType == 0){
    snprintf(stats, sizeof(stats), "Player 1 won: %d\nPlayer 2 won: %d\nGame Draw: %d",
           player_1_win, player_2_win, game_draw);
  }
  else{
    snprintf(stats, sizeof(stats), "Player 1 won: %d\nComputer won: %d\nGame Draw: %d",
           player_1_win, player_2_win, game_draw);
  }

  gtk_label_set_text(game_stats, stats);
  if(winner == 4){
    gameNotOver = 1;
  }
  else{
    gameNotOver = 0; // Mark game as over
  }
  
}

void handle_player_move(int row, int col, GtkButton *buttonTemp, int player)
{
  const char *label = (player == 1) ? "X" : "O";
  arr[row][col] = player;
  gtk_button_set_label(buttonTemp, label);
  pressed[row][col] = 1;
}

int set_playermove(int row, int col, GtkButton *buttonTemp)
{
  if (initialise != 0)
  {
    initialising(buttonTemp, row, col); // Call initializing function
    return 0;
  }

  if (pressed[row][col] || !gameNotOver) // Ignore if already pressed or game is over
    return 0;

  // Determine current player based on move counter
  int player = (++moveCounter % 2) ? 1 : 2;

  handle_player_move(row, col, buttonTemp, player); // Execute the player's move

  if (player == 1)
  {
    gtk_widget_set_name(GTK_WIDGET(buttonTemp), "player_1"); // Set X's style
  }
  else
  {
    gtk_widget_set_name(GTK_WIDGET(buttonTemp), "player_2"); // Set O's style
  }

  int winner = anywinner(arr);
  if (winner || moveCounter == 9)
  { // Check for winner or draw
    if (winner == 0 && moveCounter == 9)
    {
      update_game_status("DRAW", 0);
    }
    else if (winner == 1)
    {
      play_sound("win.wav");
      update_game_status("PLAYER 1 WON", 1);
    }
    else if (winner == 2)
    {
      play_sound("win.wav");
      update_game_status("PLAYER 2 WON", 2);
    }
    return 0; // Exit after win or draw
  }

  // Update the status for the next player's turn
  const char *next_move = (player == 1) ? "PLAYER 2'S MOVE" : "PLAYER 1'S MOVE";
  gtk_label_set_text(statusbtn, next_move);

  // If it's player 1's move and gameType is single-player, call computer move
  if (gameType == 1 && player == 1)
  {
    if (gameDifficulty == 3) // ML Mode
    {
      makeMLMove();
    }
    else 
    {
      compMove();
      if (gameNotOver) {
        gtk_label_set_text(statusbtn, "PLAYER 1'S MOVE");
      }
    }
  }
  return 0;
}

// Run when row 1, column 1 button is clicked
int on_button11_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;

  // Play sound
  play_sound("select.wav");

  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(0, 0, buttonTemp);
  return 0;
}

// Run when row 1, column 2 button is clicked
int on_button12_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;
  // Play sound
  play_sound("select.wav");

  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(0, 1, buttonTemp);
  return 0;
}

// Run when row 1, column 3 button is clicked
int on_button13_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;
  // Play sound
  play_sound("select.wav");

  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(0, 2, buttonTemp);
  return 0;
}

// Run when row 2, column 1 button is clicked
int on_button21_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;
  // Play sound
  play_sound("select.wav");

  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(1, 0, buttonTemp);
  return 0;
}

// Run when row 2, column 2 button is clicked
int on_button22_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;
// Play sound
  play_sound("select.wav");


  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(1, 1, buttonTemp);
  return 0;
}

// Run when row 2, column 3 button is clicked
int on_button23_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;
  // Play sound
  play_sound("select.wav");
  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(1, 2, buttonTemp);
  return 0;
}

// Run when row 3, column 1 button is clicked
int on_button31_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;
  // Play sound
  play_sound("select.wav");
  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(2, 0, buttonTemp);
  return 0;
}

// Run when row 3, column 2 button is clicked
int on_button32_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;
  // Play sound
  play_sound("select.wav");
  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(2, 1, buttonTemp);
  return 0;
}

// Run when row 3, column 3 button is clicked
int on_button33_clicked(GtkWidget *click_button, gpointer user_data)
{
  // Assign the generic user_data pointer to a GtkButton pointer for set_playermove function later
  GtkButton *buttonTemp = (GtkButton *)user_data;
  // Play sound
  play_sound("select.wav");
  // Run seet_playermove function to set the player click on the board and gamme
  set_playermove(2, 2, buttonTemp);
  return 0;
}

// Check if any row, column, or diagonal is all 0, 1, or 2 and return 0, 1, or 2
int anywinner(int a[3][3])
{
  // initialise count for player 1 or player 2 win
  int oneWon = 0;
  int twoWon = 0;
  int i, j;

  // Check each row for win
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      if (a[i][j] == 1)
        oneWon++; // increase counter if player 1 occupy the cells in the row
      if (a[i][j] == 2)
        twoWon++; // increase counter if player 2 occupy the cells in the row
    }
    if (oneWon == 3) // player 1 win if all cells in a row are 1
      return 1;
    if (twoWon == 3) // player 2 win if all cells in a row are 2
      return 2;

    // reset count for next row checking
    oneWon = 0;
    twoWon = 0;
  }

  // Check each column for win
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      if (a[j][i] == 1) // increase counter if player 1 occupy the cells in the column
        oneWon++;
      if (a[j][i] == 2) // increase counter if player 2 occupy the cells in the column
        twoWon++;
    }
    if (oneWon == 3) // player 1 win if all cells in a column are 1
      return 1;
    if (twoWon == 3) // player 2 win if all cells in a column are 2
      return 2;

    // reset count for next column checking
    oneWon = 0;
    twoWon = 0;
  }

  // Check left to right diagonal for win (\)
  for (i = 0; i < 3; i++)
  {
    if (a[i][i] == 1) // increase counter if player 1 occupy the cells in the diagonal
      oneWon++;
    if (a[i][i] == 2) // increase counter if player 2 occupy the cells in the diagonal
      twoWon++;
  }
  if (oneWon == 3) // player 1 win if all cells in a diagonal are 1
    return 1;
  if (twoWon == 3) // player 2 win if all cells in a diagonal are 2
    return 2;

  // reset count for next diagonal checking
  oneWon = 0;
  twoWon = 0;

  // Check left to right diagonal for win (/)
  for (i = 0, j = 2; i < 3; i++, j--)
  {
    if (a[i][j] == 1) // increase counter if player 1 occupy the cells in the right to left diagonal
      oneWon++;
    if (a[i][j] == 2) // increase counter if player 2 occupy the cells in the right to left diagonal
      twoWon++;
  }
  if (oneWon == 3) // player 1 win if all cells in a right to left diagonal are 1
    return 1;
  if (twoWon == 3) // player 2 win if all cells in a right to left diagonal are 2
    return 2;

  // return 0 if no winner
  return 0;
}

// Called after initialization is completed to reset buttons
void resetgame()
{
  int i, j;

  // reset game variable
  moveCounter = 0;
  gameNotOver = 1;
  initialise = 0;

  // loop through 3x3 board
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      // initialise the 3x3 array of button on the board
      GtkButton *buttons[3][3] = {
          {GTK_BUTTON(button11), GTK_BUTTON(button12), GTK_BUTTON(button13)},
          {GTK_BUTTON(button21), GTK_BUTTON(button22), GTK_BUTTON(button23)},
          {GTK_BUTTON(button31), GTK_BUTTON(button32), GTK_BUTTON(button33)}};
      // clear the label for each button to empty
      gtk_button_set_label(buttons[i][j], " ");

      // reset button, gameboard and pressed;
      button[i][j] = NULL;
      arr[i][j] = 0;
      pressed[i][j] = 0;
    }
  }
}

// Runs the first time each button is pressed
void initialising(GtkButton *buttonInit, int i, int j)
{
  // check if button already initialise
  if (button[i][j] != NULL)
    return;

  // set button [i][j] to the passed in variable
  button[i][j] = buttonInit;

  // gtk_button_set_label(button[i][j], "OK");

  // increament the initalise counter to track the number of initialise button
  if (++initialise == 9)
  {
    initialise = 0;
    gtk_label_set_text(statusbtn, "PLAYER 1'S MOVE");
    resetgame();
  }
}

// Random number generator: 0 < x < n
int randomNumberLessThan(int n)
{
  return (double)rand() * n / RAND_MAX;
}


void play_sound(const char *filename) {
  char path[MAX_PATH];
  snprintf(path, sizeof(path), "assets/%s", filename);
  PlaySound(path, NULL, SND_FILENAME | SND_ASYNC);
}
