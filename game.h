#include <SDL.h>
#include <SDL_video.h>
#include "general_game.h"
#include "controls.h"
#include "save_game.h"

#define RESTART "Rsestart"
#define QUIT "Quit"
#define CANCEL "Cancel"
#define MAIN_MENU "Main Menu"
#define SAVE "Save Game"
#define LOAD "Load Game"
#define DIFF "Difficulties"
#define PAUSE "Pause"
#define CHOOSE_GAME "Choose Game"
#define DIFFICALTY "Difficulty"
#define SAVE_SLOT "Save Slot"
#define LOAD_SLOT "Load Slot"
#define GAME_NAME "Game"
#define AI_1 "AI Vs AI"
#define AI_2 "Human Vs AI"
#define AI_3 "AI Vs Human"
#define AI_4 "Human Vs Human"

#define AI_SIGN 6
#define START_SIGN 5
#define DIFF_SIGN 4
#define SAVE_SIGN 3
#define LOAD_SIGN 2
#define MAIN_SIGN 1


int SDL_Init(Uint32 flags);
element_cntrl get_default_ui_tree(game *cur_game);
element_cntrl mainMenuWindow();
game* runWindow(int mainORLoad,game** prevGame);
element_cntrl game_init(game **cur_game,int mainORLoad);
element_cntrl draw_game (game *cur_game,element_cntrl prev_ui_tree);
element_cntrl startWindow();
void newButtonGeneric(linked_list_cntrl fathersList,int x,int y,char* caption,void (*pressedButton)(void* cur_game,void* ui_tree,int *quit,void* test_event),int buttonChoise);
element_cntrl choiseWindow(int iterationNum,void (*buttonAction)(void* cur_game,void* ui_tree,int *choise,void* test_event),char** captionStart);
void addNewControlToList(control* control,linked_list_cntrl fathersList);
char** initialazeChoiseWindow(void (**pressedButton)(void* cur_game,void* ui_tree,int *quit,void* test_event),int *iterationNum,game **prevGame,int flag);
void newImage(linked_list_cntrl fathersList,int x,int y,char* caption);

