#ifndef CONNECT_4_UI_H
#define CONNECT_4_UI_H
#include "controls.h"

/* game parameters */
#define CONNECT4_ROWS		6 
#define CONNECT4_COLS		7
#define CONNECT4_PLAYER_1	1
#define CONNECT4_PLAYER_2	-1

/* ui definitions */
#define C4_XOFFSET			36
#define C4_YOFFSET			78
#define C4_WGRID			700
#define C4_HGRID			700
#define C4_HBTN				88 
#define C4_WBTN				88
#define C4_PIECES_PANEL_W	600
#define C4_PIECES_PANEL_H	600

/* graphics paths */ 
#define C4_GRIDPATH					"./gfx/C4board.bmp"
#define C4_BTN_RED_PATH				"./gfx/redC4.bmp"
#define C4_BTN_BLUE_PATH			"./gfx/blueC4.bmp"
#define C4_BTN_RED_VICTORY_PATH		"./gfx/redC4victory.bmp"
#define C4_BTN_BLUE_VICTORY_PATH	"./gfx/blueC4victory.bmp"

/* functions */
element_cntrl	C4_panel_function(int* game_state,int  (*handle_next_move)(int *quit,SDL_Event* test_event));
int				color_c4(int* game_state,int player,element_cntrl ui_tree);
int				c4_set_victory_control(int i,int j,element_cntrl game_panel,int player);

#endif