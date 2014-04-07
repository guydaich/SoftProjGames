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
	element_cntrl	(*panel_function)(int* game_state,void  (*makeMove)(int *choice,SDL_Event* test_event));
	int				(*handle_mouse_button_down)(SDL_Event *event, int* game_state, int player);
	int				(*player_has_moves)(int* game_state, int player);
	int				(*is_game_over)(int* game_state);
	int				(*is_victory)(int* game_state);
	void			(*victoryColor)(int* game_state,int player,element_cntrl ui_tree);
	int				(*handle_computer_move)(int* game_state, int depth,int player);


} game;

game* new_game(int game_id);
void  restartGame(int *choice,SDL_Event* test_event);
void  quitGame(int *choice,SDL_Event* test_event);
void  goToMainMenu(int *choice,SDL_Event* test_event);
void  saveGame(int *choice,SDL_Event* test_event);
void  makeMove(int *choice,SDL_Event* test_event);
void  setDifficalty(int *choice,SDL_Event* test_event);
void  chooseGame(int *choice,SDL_Event* test_event);
void  runLoadManu(int *choice,SDL_Event* test_event);
void  loadGame(int *choice,SDL_Event* test_event);
void  runsaveManu(int *choice,SDL_Event* test_event);
void  runDiffManu(int *choice,SDL_Event* test_event);
void  runStartManu(int *choice,SDL_Event* test_event);
void  setmultiplayer(int *choice,SDL_Event* test_event);
void  setUnpause(int *choice,SDL_Event* test_event);

#endif
