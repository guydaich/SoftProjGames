#ifndef WINDOWS_DRAWINGS_H
#define WINDOWS_DRAWINGS_H
#include <SDL.h>
#include <SDL_video.h>
#include "general_game.h"
#include "save_game.h"
#include "buttonActions.h"

/* constants */
#define RESTART "Rsestart"
#define QUIT "Quit"
#define CANCEL "Cancel"
#define MAIN_MENU "Main Menu"
#define SAVE "Save Game"
#define LOAD "Load Game"
#define DIFFP1 "P1 Difficulties"
#define DIFFP2 "P2 Difficulties"
#define PAUSE "Pause"
#define CHOOSE_GAME "Choose Game"
#define DIFFICALTY "Difficulty"
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
#define P1 " Player 1"
#define P2 " Player 2"
#define P_COLORED "./gfx/coloredPlayer.bmp"
#define P_EMPTY "./gfx/emptyPlayer.bmp"

enum notification_type {
	OVERWRITE_SIGN,
	OK_SIGN
};
typedef enum notification_type notification_type;

/*TODO : explain meanings of signs*/
enum user_selection_type {
	MAIN_SIGN,
	LOAD_SIGN,
	SAVE_SIGN,
	DIFF1_SIGN,
	DIFF2_SIGN,
	START_SIGN,
	AI_SIGN,
	ZERO_SIGN
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