#ifndef REVERSI_UI_H
#define REVERSI_UI_H
#include "controls.h"

/* game definitions */
#define REVERSI_ROWS		8 
#define REVERSI_COLS		8
#define REVERSI_NO_PLAYER	0
#define REVERSI_PLAYER_1	1
#define REVERSI_PLAYER_2	-1

/* UI definitions*/
#define RVR_XOFFSET					49
#define RVR_YOFFSET					49
#define RVR_WGRID					600
#define RVR_HGRID					600
#define RVR_HBTN					75 
#define RVR_WBTN					75 
#define RVR_BUTTON_PANEL_OFFSET_X	49
#define RVR_BUTTON_PANEL_OFFSET_Y	49
#define RVR_BUTTON_PANEL_WIDTH		598
#define RVR_BUTTON_PANEL_HEIGHT		598
#define RVR_MAIN_PANEL_OFFSET_X		0
#define RVR_MAIN_PANEL_OFFSET_Y		0
#define RVR_MAIN_PANEL_WIDTH		700
#define RVR_MAIN_PANEL_HEIGHT		700

/* paths */
#define RVR_GRIDPATH		"./gfx/reversi_board.bmp"
#define RVR_BTN_WHITE_PATH	"./gfx/reversi_piece_white.bmp"
#define RVR_BTN_BLACK_PATH	"./gfx/reversi_piece_black.bmp"

/* functions */
element_cntrl	rv_panel_function(int* game_state,int  (*handle_next_move)(int *quit,SDL_Event* test_event));
int color_rv(int* game_state,int player,element_cntrl ui_tree);
#endif