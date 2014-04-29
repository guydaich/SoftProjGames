/* button_action.h							*/
/* this class holds all handlers for on_click events		*/
/* for all buttons (and grid click) in all menus in the game	*/

#include "general_game.h"

/* save game messages and files*/
#define SAVE_FILE_PREFIX		"./save_file_"
#define SAVE_FILE_SUFFIX		".in"
#define SAVE_FILE_MAX_DIGITS	1
#define OVER_WRITE_MSG			"This file already exist.\nDo you wish to overwrite?\n"
#define SAVE_GAME_ERROR_MSG		"The saved game is either corrupt or doesn't exist\n"

/* ui defintions */
#define BTN_X_OFFSET			35
#define BTN_Y_OFFSET			20
#define BTN_H					90
#define PADDING					10


/* main menu handlers */
int  go_to_game_selection(int *choice,SDL_Event* test_event);
int  go_to_load_menu(int *choice,SDL_Event* test_event);
int  quit_game(int *choice,SDL_Event* test_event);

/* game button handlers */
int  handle_next_move(int *choice,SDL_Event* test_event);
int  add_ui_tree_victory();
int  restart_game(int *choice,SDL_Event* test_event);
int  go_to_save_menu(int *choice,SDL_Event* test_event);
int  set_unpause(int *choice,SDL_Event* test_event);
int  go_to_start_menu(int *choice,SDL_Event* test_event);
int  go_to_difficulty_player1(int *choice,SDL_Event* test_event);
int  go_to_difficulty_player2(int *choice,SDL_Event* test_event);

/*game options*/
int  set_game_player_types(int *choice,SDL_Event* test_event);
int  set_player1_difficulty(int *choice,SDL_Event* test_event);
int  set_player2_difficulty(int *choice,SDL_Event* test_event);

/* save, load handlers */
int  save_game(int *choice,SDL_Event* test_event);
int  load_game(int *choice,SDL_Event* test_event);

/* game selction menu */
int  set_current_game(int *choice,SDL_Event* test_event);






