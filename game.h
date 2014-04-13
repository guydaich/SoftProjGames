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
#define DIFFP1 "P1 Difficulties"
#define DIFFP2 "P2 Difficulties"
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
	MAIN_SIGN,LOAD_SIGN,SAVE_SIGN,DIFF1_SIGN,DIFF2_SIGN,START_SIGN,AI_SIGN,ZERO_SIGN
};
typedef enum choiseWindowSign choiseWindowSign;


int SDL_Init(Uint32 flags);
int get_default_ui_tree();
element_cntrl mainMenuWindow();
int runWindow(choiseWindowSign mainORLoad);
int game_init(choiseWindowSign mainORLoad);
int draw_game ();
element_cntrl startWindow();
element_cntrl choiseWindow(int iterationNum,int (*buttonAction)(int *choise,SDL_Event* test_event),char** captionStart);
char** initialazeChoiseWindow(int (**pressedButton)(int *quit,SDL_Event* test_event),int *iterationNum,choiseWindowSign flag);
int askWindow(char *qustion,qustionWindowsSgin flag);
int qustionORtext(qustionWindowsSgin flag);
#endif