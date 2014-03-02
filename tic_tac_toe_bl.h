#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include <stdio.h>
#include "minimax.h"
#include "controls.h"

#define TIC_TAC_TOE_ROWS 3 
#define TIC_TAC_TOE_COLS 3
#define TTC_PLAYER_1 1
#define TTC_PLAYER_2 -1

/* functions */
char*			ttc_get_name();
int*			ttc_get_initial_state();
linked_list		ttc_get_state_children(int* game_state, int player);
int				ttc_get_state_score(int* game_state,int player);
int*			ttc_get_difficulty_levels();
void			ttc_draw_function();
element_cntrl ttc_panel_function(int* game_state,void  (*makeMove)(void* cur_game,element_cntrl* ui_tree,int *quit,SDL_Event* test_event));

int				ttc_is_victory(int* game_state, int player);
int				ttc_is_board_full(int* game_state);
int				ttc_add_to_children_list(linked_list list, int* game_state, int row, int col, int player);
int*			ttc_copy_and_make_move(int* game_state, int move_row, int move_col, int player);
int				ttc_make_move(int* game_state, int row, int col, int player);
int				ttc_is_game_over(int* game_state);
int				ttc_handle_mouse_button_down (SDL_Event *event,element_cntrl root, int* game_state,int player);
int				ttc_handle_computer_turn(int* game_state, int depth);

#endif