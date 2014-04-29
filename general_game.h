/* general_game.h */
/* we define here the general_game struct, that is an abstract class 
 * instantiated with some specific game, that fullfills it's interface, 
 * so that all game actions may be handled generically */

#ifndef GENERAL_GAME_H
#define GENERAL_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include "tic_tac_toe_bl.h"
#include "reversi_bl.h"
#include "connect4_bl.h"
#include "game_enum.h"

/*general game structure*/
typedef struct _general_game
{
	/*game info */
	int *board;
	int rows; 
	int cols; 
	int cur_player; 
	int difficultyP1;
	int difficultyP2;
	int is_multiplayer;
	int difficulty_num;

	/*general function*/
	char*			(*get_name)();
	int*			(*get_initial_state)();
	linked_list		(*get_state_children)(int* game_state, int player,int *error);
	int				(*get_state_score)(int* game_state);
	int*			(*get_difficulty_levels)();
	element_cntrl	(*panel_function)(int* game_state,int  (*handle_next_move)(int *choice,SDL_Event* test_event));
	int				(*handle_mouse_button_down)(SDL_Event *event, int* game_state, int player);
	int				(*player_has_moves)(int* game_state, int player);
	int				(*is_game_over)(int* game_state);
	int				(*is_victory)(int* game_state);
	int 			(*victoryColor)(int* game_state,int player,element_cntrl ui_tree);
	int				(*handle_computer_move)(int* game_state, int depth,int player);

} game;

/* functions */
game* new_game(whichGame game_id);
int handelAI_VS_AI(int *pause);

#endif
