#include <SDL.h>
#include <SDL_video.h>
#include "general_game.h"
#include "controls.h"
#include "save_game.h"


int SDL_Init(Uint32 flags);
element_cntrl get_default_ui_tree(game *cur_game);
element_cntrl mainMenuWindow();
game* runMainMenu();
element_cntrl game_init(game **cur_game);
element_cntrl draw_game (game *cur_game,element_cntrl prev_ui_tree);