#include <stdio.h>
#include "controls.h"
#include "minimax.h"

#define REVERSI_ROWS 8 
#define REVERSI_COLS 8
#define REVERSI_NAME "Reversi"
#define REVERSI_PLAYER_1 1
#define REVERSI_PLAYER_2 -1


/* functions */
char*			rv_get_name();
int*			rv_get_initial_state();
linked_list		rv_get_state_children(int* game_state, int player);
int				rv_get_state_score(int* game_state,int player);
int*			rv_get_difficulty_levels();
void			rv_draw_function();
element_cntrl	rv_panel_function(int* game_state);
int			rv_make_move(int * game_state, int rows, int cols, int player);
int				rv_is_valid_move (int *game_state, int player, int rows, int cols);
int*			rv_copy_and_make_move(int* game_state, int move_row, int move_col, int player);
int				rv_add_to_children_list(linked_list list, int* game_state, int row, int col, int player);
int				rv_is_game_over(int* game_state);
int			rv_handle_mouse_button_down (SDL_Event *event,element_cntrl root, int* game_state);