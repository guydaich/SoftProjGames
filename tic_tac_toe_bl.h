#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include <stdio.h>
#include "minimax.h"
#include "ttc_ui.h"

#define TIC_TAC_TOE_NAME "Tic-Tac-Toe"

/* functions */
char*			ttc_get_name();
int*			ttc_get_initial_state();
linked_list		ttc_get_state_children(int* game_state, int player,int *error);
int				ttc_get_state_score(int* game_state,int player);
int*			ttc_get_difficulty_levels();
void			ttc_draw_function();

int				ttc_is_victory(int* game_state);
int				ttc_is_victory_player(int* game_state, int player);
int				ttc_is_board_full(int* game_state);
int				ttc_add_to_children_list(linked_list list, int* game_state, int row, int col, int player);
int*			ttc_copy_and_make_move(int* game_state, int move_row, int move_col, int player);
int				ttc_make_move(int* game_state, int row, int col, int player);
int				ttc_is_game_over(int* game_state);
int				ttc_handle_mouse_button_down (SDL_Event *event, int* game_state,int player);
int				ttc_handle_computer_turn(int* game_state, int depth,int player);
int				ttc_is_any_victory(int* game_state);
int				ttc_player_has_moves(int* game_state);
#endif


