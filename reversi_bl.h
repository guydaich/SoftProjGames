#ifndef REVERSI_H
#define REVERSI_H

#include <stdio.h>
#include "controls.h"
#include "minimax.h"

#define REVERSI_ROWS 8 
#define REVERSI_COLS 8
#define REVERSI_NO_PLAYER 0
#define REVERSI_PLAYER_1 1
#define REVERSI_PLAYER_2 -1
#define PLAYER_1_START_1 3*REVERSI_ROWS + 3
#define PLAYER_2_START_1 4*REVERSI_ROWS + 3
#define PLAYER_1_START_2 4*REVERSI_ROWS + 4
#define PLAYER_2_START_2 3*REVERSI_ROWS + 4

#define R5 10
#define R4 -5
#define R3 5
#define R2 -1
#define R1 1




/* functions */
char*			rv_get_name();
int*			rv_get_initial_state();
linked_list		rv_get_state_children(int* game_state, int player);
int				rv_get_state_score(int* game_state,int player);
int*			rv_get_difficulty_levels();
void			rv_draw_function();
element_cntrl rv_panel_function(int* game_state,void  (*makeMove)(void* cur_game,element_cntrl* ui_tree,int *quit,SDL_Event* test_event));
int				rv_make_move(int * game_state, int rows, int cols, int player);
int				rv_is_valid_move (int *game_state, int player, int rows, int cols);
int*			rv_copy_and_make_move(int* game_state, int move_row, int move_col, int player);
int				rv_add_to_children_list(linked_list list, int* game_state, int row, int col, int player);
int				rv_is_game_over(int* game_state);
int				rv_handle_mouse_button_down (SDL_Event *event,element_cntrl root, int* game_state,int player);
int				rv_is_victory(int* game_state);
int				rv_handle_computer_turn(int* game_state, int depth,int player);
int				rv_player_has_moves(int* game_state, int player);

#endif
