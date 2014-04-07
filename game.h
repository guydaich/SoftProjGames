#include <SDL.h>
#include <SDL_video.h>
#include "general_game.h"
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
#define YES "yes"
#define NO "no"
#define OK "ok"

#define AI_SIGN 6
#define START_SIGN 5
#define DIFF_SIGN 4
#define SAVE_SIGN 3
#define LOAD_SIGN 2
#define MAIN_SIGN 1

#define OK_SIGN 2
#define OVERWRITE_SIGN 1

int SDL_Init(Uint32 flags);
void get_default_ui_tree();
element_cntrl mainMenuWindow();
void runWindow(int mainORLoad);
void game_init(int mainORLoad);
void draw_game ();
element_cntrl startWindow();
void newButtonGeneric(linked_list_cntrl fathersList,int x,int y,char* caption,void (*pressedButton)(int *choice,SDL_Event* test_event),int buttonChoise);
element_cntrl choiseWindow(int iterationNum,void (*buttonAction)(int *choise,SDL_Event* test_event),char** captionStart);
void addNewControlToList(control* control,linked_list_cntrl fathersList);
char** initialazeChoiseWindow(void (**pressedButton)(int *quit,SDL_Event* test_event),int *iterationNum,int flag);
void newImage(linked_list_cntrl fathersList,int x,int y,char* caption);
int askWindow(char *qustion,int flag);
void qustionORtext(int flag);


