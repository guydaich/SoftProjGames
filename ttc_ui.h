#ifndef TIC_TAC_TOE_UI_H
#define TIC_TAC_TOE_UI_H
#include "controls.h"

/*ui defintions */
#define TTC_XOFFSET					50
#define TTC_YOFFSET					50
#define TTC_WGRID					600
#define TTC_HGRID					600
#define TTC_HBTN					200 
#define TTC_WBTN					200 
#define TTC_BUTTON_PANEL_OFFSET_X	50
#define TTC_BUTTON_PANEL_OFFSET_Y	50
#define TTC_BUTTON_PANEL_WIDTH		600
#define TTC_BUTTON_PANEL_HEIGHT		600
#define TTC_MAIN_PANEL_OFFSET_X		0
#define TTC_MAIN_PANEL_OFFSET_Y		0
#define TTC_MAIN_PANEL_WIDTH		700
#define TTC_MAIN_PANEL_HEIGHT		700

/*game definitions */
#define TIC_TAC_TOE_ROWS 3 
#define TIC_TAC_TOE_COLS 3
#define TTC_PLAYER_1 1
#define TTC_PLAYER_2 -1

/* paths */
#define TTC_GRIDPATH			"./gfx/ttc_board_mag.bmp"
#define TTC_BTNXPATH			"./gfx/ttc_x_mag.bmp"
#define TTC_BTNOPATH			"./gfx/ttc_o_mag.bmp"
#define TTC_BTNEPATH			"./gfx/ttc_empty.bmp"
#define TTC_BTNXPATH_VICTORY	"./gfx/ttc_x_magVictory.bmp"
#define TTC_BTNOPATH_VICTORY	"./gfx/ttc_o_magVicrory.bmp"

/* functions */
element_cntrl	ttc_panel_function(int* game_state,int  (*handle_next_move)(int *quit,SDL_Event* test_event));
int				set_victory_control(int i,int j,element_cntrl game_panel,int player);
int				color_ttc(int* game_state,int player,element_cntrl ui_tree);
#endif