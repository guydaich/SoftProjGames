#ifndef SAVE_GAME_H
#define SAVE_GAME_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "game_enum.h"

/* constants */
#define MAX_NAME_SIZE 12
#define CONNECT4_COLS 7
#define CONNECT4_GROWS 6
#define TTT_COLS 3
#define TTT_ROWS 3
#define REVERSI_COLS 8
#define REVERSI_ROWS 8

/* function */
int write_game_to_file(char* filename, int *game_state, int player, int cols, int rows,char *new_game_name);
int load_game_from_file(char* filename, whichGame* whichG,int** board,int *player);
int fill_matrix_loaded(FILE *file,int *game_matrix,int cols,int rows);
int save_game_in_file(char* filename, int *game_state, int player, int cols, int rows,char *new_game_name);
#endif