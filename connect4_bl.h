#ifndef CONNECT_4_H
#define CONNECT_4_H
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "minimax.h"
#include "connect4_ui.h"

#define Connect4_NAME "Connect4"

extern int boardCount;
extern vertex mainRoot;

char*			get_name_C4();
int *			get_initial_state_C4();
linked_list		get_state_children_C4(int* matrix, int player,int *error);
int				get_state_score_C4(int* game_matrix,int player);
int*			get_difficulty_levels_C4();

int				is_victory_C4(int* game_state);
int				add_to_children_list_C4(linked_list list, int* game_state, int row, int col, int player);
int *			copy_and_make_move_C4(board_t from,int move_row, int move_col, int player);
int				C4_make_move(int* game_state, int row, int col, int player);
int				is_game_over_C4(int* game_state);
int				C4_handle_mouse_button_down (SDL_Event *event, int* game_state,int player);
int				C4_handle_computer_turn(int* game_state, int depth,int player);
#endif