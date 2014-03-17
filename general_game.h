#ifndef GENERAL_GAME_H
#define GENERAL_GAME_H

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
	int difficulty_num;

	/*general function*/
	char*			(*get_name)();
	int*			(*get_initial_state)();
	linked_list		(*get_state_children)(int* game_state, int player);
	int				(*get_state_score)(int* game_state,int player);
	int*			(*get_difficulty_levels)();
	element_cntrl	(*panel_function)(int* game_state,void  (*makeMove)(void* cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event));
	int				(*handle_mouse_button_down)(SDL_Event *event,element_cntrl root, int* game_state, int player);
	int				(*player_has_moves)(int* game_state, int player);
	int				(*is_game_over)(int* game_state);
	int				(*is_victory)(int* game_state,int player);
	int				(*handle_computer_move)(int* game_state, int depth,int player);


} game;

game* new_game(int game_id);
void  restartGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  quitGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  goToMainMenu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  saveGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  makeMove(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  setDifficalty(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  chooseGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  runLoadManu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  loadGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  runsaveManu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  runDiffManu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  runStartManu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  setmultiplayer(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);
void  setUnpause(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event);

#endif
