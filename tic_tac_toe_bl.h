#include <stdio.h>
#include "minimax.h"
#include "controls.h"

#define TIC_TAC_TOE_ROWS 3 
#define TIC_TAC_TOE_COLS 3
#define TTC_PLAYER_1 1
#define TTC_PLAYER_2 -1

/* functions */
char*		get_name();
int*		get_initial_state();
linked_list get_state_children(int* game_state, int player);
int get_state_score(int* game_state,int player);
int*		get_difficulty_levels();
void		draw_function();
element_cntrl panel_function(int* game_state);

int is_victory(int* game_state, int player);
int is_board_full(int* game_state);
int add_to_children_list(linked_list list, int* game_state, int row, int col, int player);
int* copy_and_make_move(int* game_state, int move_row, int move_col, int player);
void make_move(int* game_state, int row, int col, int player);
int is_game_over(int* game_state);
void ttc_handle_mouse_button_down (SDL_Event *event,element_cntrl root, int* game_state);