#include "save_game.h"

void save_game_to_file(char* filename, int *game_state, int player, int cols, int rows,char *gameName)
{
	FILE *file; 
	int i=0,j=0;
	
	/* open file for writing */
	file = fopen(filename, "w");
	if (file == NULL)
	{ // complete
		perror("Stand"); 
		exit(0);
	}
	fprintf(file, "%s\n",gameName);//print Game name
	fprintf(file, "%d\n",player);//print player
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fprintf(file,"%d ",game_state[i*cols+j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

int* load_game_from_file(char* filename, int* whichGame){
	FILE *file; 
	char getName[MAX_NAME_SIZE];
	int *gameBoard;
	int player;

	file = fopen(filename, "r");
	if (file == NULL)
	{ // complete
		perror("Stand"); 
		exit(0);
	}
	fscanf(file,"%s",getName);
	printf("%s\n",getName);
	if(strcmp(getName,"Connect4")==0){
		*whichGame=3;
		gameBoard=(int*)calloc(CONNECT4_COLS*CONNECT4_GROWS,sizeof(int));
		fscanf(file,"%d",&player);
		fill_matrix_loaded(file,gameBoard,CONNECT4_COLS,CONNECT4_GROWS);
		//set functions to curr game functions and game board and player
	}
	if(strcmp(getName,"Tic-Tac-Toe")==0){
		*whichGame=1;
		gameBoard=(int*)calloc(TTT_COLS*TTT_ROWS,sizeof(int));
		fscanf(file,"%d",&player);
		fill_matrix_loaded(file,gameBoard,TTT_COLS,TTT_ROWS);
		//set functions to curr game functions and game board and player
	}
	if(strcmp(getName,"Reversi")==0){
		*whichGame=2;
		gameBoard=(int*)calloc(REVERSI_COLS*REVERSI_ROWS,sizeof(int));
		fscanf(file,"%d",&player);
		fill_matrix_loaded(file,gameBoard,REVERSI_COLS,REVERSI_ROWS);
		//set functions to curr game functions and game board and player
	}
	return gameBoard;
}

void fill_matrix_loaded(FILE *file,int *gameMatrix,int cols,int rows){
	int i,j,temp;
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fscanf(file,"%d ",&temp);
			gameMatrix[i*(cols)+j]=temp;
		}
	}
}