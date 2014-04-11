#ifndef GAME_H
#define GAME_H
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

enum qustionWindowsSgin {
	OVERWRITE_SIGN,OK_SIGN
};
typedef enum qustionWindowsSgin qustionWindowsSgin;

enum choiseWindowSign {
	MAIN_SIGN,LOAD_SIGN,SAVE_SIGN,DIFF_SIGN,START_SIGN,AI_SIGN
};
typedef enum choiseWindowSign choiseWindowSign;


int SDL_Init(Uint32 flags);
void get_default_ui_tree();
element_cntrl mainMenuWindow();
void runWindow(choiseWindowSign mainORLoad);
void game_init(choiseWindowSign mainORLoad);
void draw_game ();
element_cntrl startWindow();
element_cntrl choiseWindow(int iterationNum,void (*buttonAction)(int *choise,SDL_Event* test_event),char** captionStart);
char** initialazeChoiseWindow(void (**pressedButton)(int *quit,SDL_Event* test_event),int *iterationNum,choiseWindowSign flag);
void newImage(linked_list_cntrl fathersList,int x,int y,char* caption);
int askWindow(char *qustion,qustionWindowsSgin flag);
void qustionORtext(qustionWindowsSgin flag);
#endif