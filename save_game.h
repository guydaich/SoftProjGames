#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_SIZE 12

#define CONNECT4_COLS 7
#define CONNECT4_GROWS 6

#define TTT_COLS 3
#define TTT_ROWS 3

#define REVERSI_COLS 8
#define REVERSI_ROWS 8


int write_game_to_file(char* filename, int *game_state, int player, int cols, int rows,char *gameName);
int load_game_from_file(char* filename, int* whichGame,int** board);
int fill_matrix_loaded(FILE *file,int *gameMatrix,int cols,int rows);
int saveGameinFile(char* filename, int *game_state, int player, int cols, int rows,char *gameName);

