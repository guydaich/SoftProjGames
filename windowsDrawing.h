/* windows_drawing.h									*/
/* this class handles the drawing of all windows throughout the game.		*/
/* These include Menus for selections, game windows, and notifications	*/

#ifndef WINDOWS_DRAWINGS_H
#define WINDOWS_DRAWINGS_H
#include <SDL.h>
#include <SDL_video.h>
#include "general_game.h"
#include "save_game.h"
#include "buttonActions.h"

/* constants */
#define RESTART "Restart"
#define QUIT "Quit"
#define CANCEL "Cancel"
#define MAIN_MENU "Main Menu"
#define SAVE "Save Game"
#define LOAD "Load Game"
#define DIFFP1 "P1 Difficulties"
#define DIFFP2 "P2 Difficulties"
#define PAUSE "Pause"
#define USPAUSE "Unpause"
#define CHOOSE_GAME "Choose Game"
#define DIFFICULTYP1 "Difficulty P1 level"
#define DIFFICULTYP2 "Difficulty P2 level"
#define SAVE_SLOT "Save Slot"
#define LOAD_SLOT "Load Slot"
#define GAME_NAME "Game"
#define AI_1 "AI Vs AI"
#define AI_2 "Human Vs AI"
#define AI_3 "AI Vs Human"
#define AI_4 "Human Vs Human"
#define YES "yes"
#define NO "no"
#define OK "ok"
#define P1 " Player 1\n"
#define P2 " Player 2\n"
#define P_COLORED "./gfx/coloredPlayer.bmp"
#define P_EMPTY "./gfx/emptyPlayer.bmp"

#define GAME_AREA_W				700
#define GAME_AREA_H				1000
#define GAME_PANEL_W			300
#define GAME_PANEL_H			1000
#define BTN_X_OFFSET			25
#define BTN_Y_OFFSET			20
#define BTN_H					100
#define PADDING					10
#define PLAYER_LABEL_X_OFFSET	160
#define PLAYER_LABEL_X_PADDING	300
#define PLAYER_LABEL_Y_OFFSET	2
#define PLAYER_LABEL_H			30
#define PLAYER_LABEL_W			100
#define BTN_PANEL_X				0
#define BTN_PANEL_Y				0
#define BTN_PANEL_W				250
#define BTN_PANEL_H				60
#define BTN_PANEL_R				220
#define BTN_PANEL_G				220
#define BTN_PANEL_B				220
#define BTN_X					20
#define BTN_Y					20
#define BTN_X_PAD				50
#define MAIN_X					0
#define MAIN_Y					0
#define MAIN_W					240
#define MAIN_H					300
#define CHOICE_WIN_X			0
#define CHOICE_WIN_Y			0
#define CHOICE_WIN_W			250
#define CHOICE_WIN_H			50
#define START_PANEL_X			0
#define START_PANEL_Y			0
#define START_PANEL_W			240
#define START_PANEL_H			300
#define START_BTN_X				20
#define START_BTN_Y				20
#define START_BTN_H				60	
#define QUESTION_X				0
#define QUESTION_Y				0
#define QUESTION_W				500
#define QUESTION_H				250
#define NOTIF_X					170
#define NOTIF_Y					50
#define NOTIF_PAD				140
#define TEXTAREA_PATH			"./gfx/textArea.bmp"
#define BUTTON_LABEL_PATH		"./gfx/button_label.bmp"



/* represent the type of notification window */
enum notification_type {
	USER_PROMPT,	/* ask user for Yes/No Answer*/
	USER_NOTIF		/* only notify user */
};
typedef enum notification_type notification_type;

/* represent the type of multiple choice window */
enum user_selection_type {
	SELECT_GAME,			/* for game selection menu */
	LOAD_GAME,				/* for game load menu */
	SAVE_GAME,				/* for game save menu*/
	SET_DIFF_PLAYER1,		/* for player 1 diffficulty selection */
	SET_DIFF_PLAYER2,		/* for player 2 difficulty selection */
	SELECT_MAIN_MENU,		/* for main menu */
	SELECT_PLAYER_TYPES,	/* player types (Human, AI) selectiom menu*/
	EMPTY_WINDOW			/* empty window, for cases where a selection isn't nescarry (like HUMAN v HUMAN) diff*/
};
typedef enum user_selection_type user_selection_type;

/* SDL */
int				SDL_Init(Uint32 flags);

/* general ui */
int				get_default_ui_tree();
int				draw_game();

/* windows */
element_cntrl	main_menu_window();
int				run_window(user_selection_type start_type);
int				game_init(user_selection_type start_type);
element_cntrl	start_window();
element_cntrl	choice_window(int iter_num,int (*buttonAction)(int *choise,SDL_Event* test_event),char** captionStart);
char**			init_choice_window(int (**pressed_button)(int *quit,SDL_Event* test_event),int *iter_num,user_selection_type flag);
int				question_window(char *qustion,notification_type flag);
int				notfication_window(notification_type flag);

#endif