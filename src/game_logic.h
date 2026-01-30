#ifndef GAME_GLOBALS_H
#define GAME_GLOBALS_H

#include <gtk/gtk.h>
#include <windows.h>

// Global widget declarations
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

// Global button and game variables
extern GtkButton *diffClick, *gameClick;
extern GtkButton *button[3][3];

// Global flags
extern int flag, gameNotOver, initialise, statusFlag, pressed[3][3];
extern int moveCounter, total_game_played, player_1_win, player_2_win, game_draw;

// Game parameters
extern int arr[3][3];
extern int gameType, gameDifficulty;

void on_pvpstartbutton_clicked(GtkWidget *click_button, gpointer user_data);
void on_pvcstartbutton_clicked(GtkWidget *click_button, gpointer user_data);
void resetgame();
int anywinner(int a[3][3]);
void play_sound(const char *filepath);
int set_playermove(int row, int col, GtkButton *buttonTemp);
void update_game_status(const char *status_text, int winner);
int randomNumberLessThan(int n);
void initialising(GtkButton *buttonInit, int i, int j);
void makeMLMove();
void compMove();

#endif // GAME_GLOBALS_H

