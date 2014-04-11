#include "save_game.h"
//<unistd.h>


int saveGameinFile(char* filename, int *game_state, int player, int cols, int rows,char *gameName){
	FILE *file;

	file = fopen(filename, "r");
	if(file!=NULL){
		fclose(file);
		return 1;
	}
	else {
		if(write_game_to_file(filename,game_state,player,cols,rows,gameName)==0) return 0;
		else return -1;
	}
}

int write_game_to_file(char* filename, int *game_state, int player, int cols, int rows,char *gameName)
{
	FILE *file; 
	int i=0,j=0;
	
	/* open file for writing */
	file = fopen(filename, "w");
	if (file == NULL)//the file already exists
	{ 
		return -1;
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
	return 0;
}

int load_game_from_file(char* filename, whichGame* whichG,int** board,int *player){
	FILE *file; 
	char getName[MAX_NAME_SIZE];
	int *gameBoard;
	int error;

	file = fopen(filename, "r");
	if (file == NULL)
	{ 
		return -2;
	}
	fscanf(file,"%s",getName);
	printf("%s\n",getName);
	fscanf(file,"%d",player);
	if (*player!=-1 && *player!=1){
		return -2;
	}
	else if(strcmp(getName,"Connect4")==0){
		*whichG=CONNECT4;
		gameBoard=(int*)calloc(CONNECT4_COLS*CONNECT4_GROWS,sizeof(int));
		if (gameBoard==NULL){
			return -1;
		}
		error=fill_matrix_loaded(file,gameBoard,CONNECT4_COLS,CONNECT4_GROWS);
		//set functions to curr game functions and game board and player
	}
	else if(strcmp(getName,"Tic-Tac-Toe")==0){
		*whichG=TTC;
		gameBoard=(int*)calloc(TTT_COLS*TTT_ROWS,sizeof(int));
		if (gameBoard==NULL){
			return -1;
		}
		error=fill_matrix_loaded(file,gameBoard,TTT_COLS,TTT_ROWS);
		//set functions to curr game functions and game board and player
	}
	else if(strcmp(getName,"Reversi")==0){
		*whichG=REVERSI;
		gameBoard=(int*)calloc(REVERSI_COLS*REVERSI_ROWS,sizeof(int));
		if (gameBoard==NULL){
			return -1;
		}
		error=fill_matrix_loaded(file,gameBoard,REVERSI_COLS,REVERSI_ROWS);
		//set functions to curr game functions and game board and player
	}
	else{
		return -2;
	}
	if (error==-1){
		return -2;
	}
	*board=gameBoard;
	return 0;
}

int fill_matrix_loaded(FILE *file,int *gameMatrix,int cols,int rows){
	int i,j,temp;
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fscanf(file,"%d ",&temp);
			if (temp!=1 && temp!=0 && temp!=-1)
				return -1;
			gameMatrix[i*(cols)+j]=temp;
		}
	}
	return 0;
}