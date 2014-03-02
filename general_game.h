#include <stdio.h>
#include <stdlib.h>
#include "tic_tac_toe_bl.h"
#include "reversi_bl.h"
#include "connect4_bl.h"

#define TTC 1
#define REVERSI 2
#define CONNECT4 3

/*general game structure*/
typedef struct _general_game
{
	/*game info */
	int *board;
	int rows; 
	int cols; 
	int cur_player; 
	int difficulty;
	int is_multiplayer;

	/*general function*/
	char*			(*get_name)();
	int*			(*get_initial_state)();
	linked_list		(*get_state_children)(int* game_state, int player);
	int				(*get_state_score)(int* game_state,int player);
	int*			(*get_difficulty_levels)();
	element_cntrl	(*panel_function)(int* game_state);
	int				(*handle_mouse_button_down)(SDL_Event *event,element_cntrl root, int* game_state, int player);
	int				(*player_has_moves)(int* game_state, int player);
	int				(*is_game_over)(int* game_state);
	int				(*is_victory)(int* game_state);
	int				(*handle_computer_move)(int* game_state);


} game;

game* new_game(int game_id);
