#include "controls.h"

#define CONNECT4_ROWS 6 
#define CONNECT4_COLS 7
#define CONNECT4_PLAYER_1 1
#define CONNECT4_PLAYER_2 -1

#define C4_XOFFSET 36
#define C4_YOFFSET 78
#define C4_WGRID 700
#define C4_HGRID 700
#define C4_HBTN 88 
#define C4_WBTN 88 

#define C4_GRIDPATH "./gfx/C4board.bmp"
#define C4_BTN_RED_PATH "./gfx/redC4.bmp"
#define C4_BTN_BLUE_PATH "./gfx/blueC4.bmp"

element_cntrl	C4_panel_function(int* game_state,void  (*makeMove)(int *quit,SDL_Event* test_event));

