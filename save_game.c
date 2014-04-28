#include "save_game.h"

/* given a game specifier, game state and file name
 * function creates a saved-game file in provided file path
 * returns 0 on success, -1 on save failurem and 1 if file not available*/
int save_game_in_file(char* filename, int *game_state, int player, int cols, int rows,char *game_name)
{
	int statCheck=0;
	/*open read, check existence*/
	statCheck=access (filename,R_OK);
	if(statCheck==0){
		return 1;
	}
	else {
		/*try save*/
		if(write_game_to_file(filename,game_state,player,cols,rows,game_name)==0) 
		{
			if (file != NULL){
				fclose(file);
			}
			return 0;
		}
		else {
		return -1;
		}
	}
}

/*given a game specifier, game state and file name
 * function creates a saved-game file in provided file path
 * returns 0 on success, -1 on save failurem and 1 if file not available**/
int write_game_to_file(char* filename, int *game_state, int player, int cols, int rows,char *game_name)
{
	FILE *file=NULL; 
	int i=0,j=0;
	
	/* open file for writing */
	file = fopen(filename, "w");
	/* file exists, so open failed on some other issue*/
	if (file == NULL) 
	{ 
		printf("ERROR: can't open the file in order to save\n");
		return -1;
	}
	/*print Game, Player*/
	fprintf(file, "%s\n",game_name);
	fprintf(file, "%d\n",player);
	/*print board*/
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fprintf(file,"%d ",game_state[i*cols+j]);
		}
		fprintf(file, "\n");
	}
	/*close and terminate*/
	fclose(file);
	return 0;
}

/* given a save file, game specifier, and board pointer
 * loads game content to board. 
 * return 0 on success, -1 on fatal error, -2 on format, file errors */
int load_game_from_file(char* filename, whichGame* whichG,int** board,int *player)
{
	int statCheck=0;
	FILE *file=NULL;
	char new_game_name[MAX_NAME_SIZE];
	int *game_board;
	int error=0;

	/*check file existence*/
	statCheck=access (filename,W_OK);
	if (statCheck <0)
	{ 
		printf("ERROR: can't open load file\n");
		return -2;
	}
	file = fopen(filename, "r");
	if (file==NULL){
		printf("ERROR: can't open load file\n");
		return -2;
	}
	/*scan game attributes*/
	fscanf(file,"%s",new_game_name);
	printf("%s\n",new_game_name);
	fscanf(file,"%d",player);

	
	/* validate players are legal*/
	if (*player!=-1 && *player!=1){
		printf("ERROR: load file in wrong format\n");
		return -2;
	}

	/*validate game name, set game specifier to relevant game, 
	assign game board and load game state accordint to file*/
	else if(strcmp(new_game_name,"Connect4")==0){
		*whichG=CONNECT4;
		game_board=(int*)calloc(CONNECT4_COLS*CONNECT4_GROWS,sizeof(int));
		if (game_board==NULL){
			printf("ERROR: failed to allocae board in load_game_from_file\n");
			return -1;
		}
		error=fill_matrix_loaded(file,game_board,CONNECT4_COLS,CONNECT4_GROWS);
	}
	else if(strcmp(new_game_name,"Tic-Tac-Toe")==0){
		*whichG=TTC;
		game_board=(int*)calloc(TTT_COLS*TTT_ROWS,sizeof(int));
		if (game_board==NULL){
			printf("ERROR: failed to allocae board in load_game_from_file\n");
			return -1;
		}
		error=fill_matrix_loaded(file,game_board,TTT_COLS,TTT_ROWS);
	}
	else if(strcmp(new_game_name,"Reversi")==0){
		*whichG=REVERSI;
		game_board=(int*)calloc(REVERSI_COLS*REVERSI_ROWS,sizeof(int));
		if (game_board==NULL){
			printf("ERROR: failed to allocae board in load_game_from_file\n");
			return -1;
		}
		error=fill_matrix_loaded(file,game_board,REVERSI_COLS,REVERSI_ROWS);
	}
	/*game name wrong*/
	else 
	{
		printf("ERROR: load file in wrong format\n");
		return -2;
	}
	/*if game state creation failed, return error*/
	if (error==-1){
		printf("ERROR: load file in wrong format\n");
		return -2;
	}
	/*success*/
	*board=game_board;
	return 0;
}

/*given a File, matrix and sizes
* scans saved board to matrix */
int fill_matrix_loaded(FILE *file,int *game_matrix,int cols,int rows)
{
	int i,j,temp;
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fscanf(file,"%d ",&temp);
			if (temp!=1 && temp!=0 && temp!=-1)
				return -1;
			game_matrix[i*(cols)+j]=temp;
		}
	}
	if (fscanf(file,"%d ",&temp)<0){
		return 0;
	}
	else {
		return -1;
	}
}