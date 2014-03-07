#include <SDL.h>
#include <SDL_video.h>
#include "general_game.h"
#include "controls.h"
#include "save_game.h"

#define DIFF_SIGN 4
#define SAVE_SIGN 3
#define LOAD_SIGN 2
#define MAIN_SIGN 1


int SDL_Init(Uint32 flags);
element_cntrl get_default_ui_tree(game *cur_game);
element_cntrl mainMenuWindow();
game* runMainMenu(int mainORLoad,game** prevGame);
element_cntrl game_init(game **cur_game,int mainORLoad);
element_cntrl draw_game (game *cur_game,element_cntrl prev_ui_tree);
element_cntrl loadWindow();
element_cntrl saveWindow();
element_cntrl diffWindow();

