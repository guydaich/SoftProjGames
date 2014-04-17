#include "buttonActions.h"
#include "windowsDrawing.h"

int gameNum=0;

extern int quit;
extern game* cur_game;
extern element_cntrl ui_tree;

/* restarts game, according to user selection and caught sdl_event
 * returns 0 on success, -1 on failure */
int restartGame(int *choice,SDL_Event* test_event)
{
	int error=0;
	if(cur_game->board!=NULL){//free prev 
		free(cur_game->board);
	}
	cur_game->board=cur_game->get_initial_state();
	if (cur_game->board==NULL){//failed
		return -1;
	}
	cur_game->cur_player=1;
	if (cur_game->is_multiplayer==3){
		*choice=1;
	}
	error=draw_game();
	if (error<0){
		return -1;
	}
	return 0;
}

/* ends game, according to user selection and caught sdl_event
 * by ending the main while(!quit) loop */
int quitGame(int *choice,SDL_Event* test_event)
{
	quit=1;
	return 0;
}

/* changes the current window to main menu window,
 * retuns -1 on failure*/
int goToMainMenu(int *choice,SDL_Event* test_event)
{
	int error;
	//freeControlList(ui_tree);
	error=runWindow(MAIN_SIGN);
	if (error<0){
		return -1;
	}
	return 0;
}

/* saves the game, according to selected save-game
 * slot. returns -1 on failure*/
int saveGame(int *choice,SDL_Event* test_event)
{
	int onTopOf=0,userAnswer=0,error=0;
	/*allocate file path*/
	char* fileLocation=(char *)malloc(36);
	sprintf(fileLocation,"/home/davidl/load%d.txt",*choice);
	/*check is save over-writes*/
	onTopOf=save_game_in_file(fileLocation,cur_game->board,cur_game->cur_player,
						cur_game->cols,cur_game->rows,(cur_game->get_name()));
	if(onTopOf==1)
	{
         //ask user if he want s to overwrite
		freeControlList(ui_tree);
		userAnswer=askWindow("This file already exist.\nDo you wish to overwrite?",OVERWRITE_SIGN);
		if (userAnswer < 0)
		{
			printf("ERROR: failed to interact with user\n");
			free(fileLocation);
			return -1;
		}
		if (userAnswer==1){
			/*try writing*/
			error=write_game_to_file(fileLocation,cur_game->board,cur_game->cur_player,
						cur_game->cols,cur_game->rows,(cur_game->get_name()));
			/*impossible to overwrtie*/
			if(error<0){
				printf("failed to write to file after asking\n");
				free(fileLocation);
				return -1;
			}
		}
	}

	/* makes next move on board, according to current game,
 	* types of players. returns -1 on failure.*/
	else if(onTopOf<0){
		printf("failed to write to file\n");
		free(fileLocation);
		return -1;
	}
	free(fileLocation);
	return 0;
}

/*handle the move request by game implementation, and check success*/
int  makeMove(int *choice,SDL_Event* test_event)
{
	int move_success = 0,error=0;
	if (*choice==1){//paused
		return 0;
	}
	move_success = cur_game->handle_mouse_button_down(test_event, cur_game->board, cur_game->cur_player);		
	if (move_success==0){
		return 0; 
	}
	else if(move_success<0){
		printf("failed in handle_mouse_button_down\n");
		free(cur_game);
		gameNum--;
		freeControlList(ui_tree);
		return -1;
	}
	cur_game->cur_player = (-1)*cur_game->cur_player;
	if ( !cur_game->is_game_over( cur_game->board))
	{
		/* if playing against computer */
		if (cur_game->is_multiplayer==2 || cur_game->is_multiplayer==3)
		{
			/* call game implementation of minimax wrapper, 
			 * and make AI move, according to AI player */
			if (cur_game->cur_player==1){
				error=cur_game->handle_computer_move( cur_game->board,cur_game->difficultyP1,cur_game->cur_player);
				if (error<0){
					printf("failed in handle_computer_move\n");
					free(cur_game);
					gameNum--;
					freeControlList(ui_tree);
					return -1;
				}
			}
			else {
				error=cur_game->handle_computer_move( cur_game->board,cur_game->difficultyP2,cur_game->cur_player);
				if (error<0){
					printf("failed in handle_computer_move\n");
					free(cur_game);
					gameNum--;
					freeControlList(ui_tree);
					return -1;
				}
			}
			/* change turns again */
			cur_game->cur_player = (-1)*  cur_game->cur_player;
		}	
	}
	/*draw updated game UI*/
	error=draw_game();
	if (error<0){
		printf("failed in draw_game\n");
		free(cur_game);
		gameNum--;
		freeControlList(ui_tree);
		return -1;
	}
	/*check if game ended*/
	if ( cur_game->is_game_over( cur_game->board)){
		/*if victory, highlight the victorious sequence
		 * accoring to the victor*/
		if (cur_game->is_victory( cur_game->board) == 1){
			error=cur_game->victoryColor(cur_game->board,1,ui_tree);
			if (error<0){
				printf("failed in victoryColor\n");
				free(cur_game);
				gameNum--;
				freeControlList(ui_tree);
				return -1;
			}
		}
		else if (cur_game->is_victory( cur_game->board) == -1){
			error=cur_game->victoryColor(cur_game->board,-1,ui_tree);
			if (error<0){
				printf("failed in victoryColor\n");
				free(cur_game);
				gameNum--;
				freeControlList(ui_tree);
				return -1;
			}
		}
		else //if tie, add to ui a tie-message
		{
			error=newButtonGeneric(ui_tree->children,300,480,"draw",restartGame,0);
			if (error<0){
				printf("failed in makeing draw button\n");
				free(cur_game);
				gameNum--;
				freeControlList(ui_tree);
				return -1;
			}
			ui_tree->children->tail->parent=ui_tree;
		}
		/*repaint the ui*/
		if (draw_ui_tree(ui_tree)<0)
		{
			printf("ERROR: cannot draw UI tree");
			return -1;
		}
		SDL_Flip(ui_tree->cntrl->srfc);
	}
	return 0;
}

/* assign difficulty level for player 1*/
int  setDifficaltyP1(int *choice,SDL_Event* test_event)
{
	cur_game->difficultyP1=(cur_game->get_difficulty_levels())[*choice-1];
	if (cur_game->difficultyP1<1 || cur_game->difficultyP1>9){
		return -1;
	}
	return 0;
}

/* assign difficulty level for player 2*/
int  setDifficaltyP2(int *choice,SDL_Event* test_event)
{
	cur_game->difficultyP2=(cur_game->get_difficulty_levels())[*choice-1];
	if (cur_game->difficultyP2<1 || cur_game->difficultyP2>9){
		return -1;
	}
	return 0;
}

/* set the type of game: AI v AI, Human v AI, etc. */
int  setmultiplayer(int *choice,SDL_Event* test_event)
{
	cur_game->is_multiplayer=*choice;
	return 0;
}

/* handle game selection */
int  chooseGame(int *choice,SDL_Event* test_event)
{
	/* clear current game */
	if (cur_game!=NULL){
	free(cur_game);
	gameNum--;
	}
	/* assign new game */
	cur_game=new_game(*choice);
	if(cur_game==NULL){
		return -1;
	}
	cur_game->board=cur_game->get_initial_state();
	if(cur_game->board==NULL){
		return -1;
	}
	return 0;
}

/* move to load game window*/
int  runLoadManu(int *choice,SDL_Event* test_event)
{
	int error;
	//freeControlList(ui_tree);
	error=runWindow(LOAD_SIGN);
	if(error<0){
		return -1;
	}
	return 0;
}

/*move to start game window*/
int  runStartManu(int *choice,SDL_Event* test_event)
{
	int error;
	//freeControlList(ui_tree);
	error=runWindow(START_SIGN);
	if(error<0 || (cur_game==NULL && quit!=1)){
		printf("ERORR: could not move to new window");
		return -1;
	}
	else if (quit==1){
		return 0;
	}
	error=runWindow(AI_SIGN);
	if(error<0){
		return -1;
	}
	if(cur_game->is_multiplayer<0 || cur_game->is_multiplayer>4){
		return -1;
	}
	if (cur_game->is_multiplayer==1 || cur_game->is_multiplayer==3){
		*choice=1;//set puase to be 1
	}
	switch(cur_game->is_multiplayer){
		case 1:
			error=runWindow(DIFF1_SIGN);
			if(error<0){
				return -1;
			}
			error=game_init(DIFF2_SIGN);
			if(error<0){
				return -1;
			}
			break;
		case 2:
			error=game_init(DIFF2_SIGN);
			if(error<0){
				return -1;
			}
			break;
		case 3:
			error=game_init(DIFF1_SIGN);
			if(error<0){
				return -1;
			}
			break;
		case 4:
			error=game_init(ZERO_SIGN);//idea-ZERO_SIGN
			if(error<0){
				return -1;
			}
			break;
	}
	return 0;
}

/* load a game from save game file */
int loadGame(int *choice,SDL_Event* test_event)
{
	whichGame whichGame;
	int error,player;
	int *gameBoard; 
	/*assign file_path*/
	char* fileLocation=(char *)malloc(36);
	sprintf(fileLocation,"/home/davidl/load%d.txt",*choice);
	/*try loading from file*/
	error=load_game_from_file(fileLocation,&whichGame,&gameBoard,&player);
	if (error==-2){
		error = askWindow("the game is either corrupt or doesn't exist",OK_SIGN);
		if (error<0)
		{
			printf("ERROR: could not notify user of problem");
			return -1;
		}
		/* resume program */
		error=runStartManu(choice,test_event);
		if (error<0){
			return -1;
		}
		cur_game=NULL;
		return 0;
	}
	if (error==-1){
		return -1;
	}
	//free(fileLocation);
	
	/*instantiate new game*/
	cur_game=new_game(whichGame);
	if(cur_game==NULL){
		return -1;
	}

	/*assign the parsed board*/
	cur_game->board=gameBoard;
	cur_game->cur_player=player;
	return 0;
}

/* go to save menu*/
int runsaveManu(int *choice,SDL_Event* test_event)
{
	int error=0;
	//freeControlList(ui_tree);
	error=game_init(SAVE_SIGN);
	if (error<0){
		return -1;
	}
	return 0;
}

/* go to difficulty Selection menu for Player 1*/
int runDiffManuP1(int *choice,SDL_Event* test_event)
{
	int error=0;
	//freeControlList(ui_tree);
	error=game_init(DIFF1_SIGN);
	if (error<0){
		return -1;
	}
	return 0;
}

/* go to difficulty Selection menu for Player 2*/
int runDiffManuP2(int *choice,SDL_Event* test_event)
{
	int error=0;
	//freeControlList(ui_tree);
	error=game_init(DIFF2_SIGN);
	if (error<0){
		return -1;
	}
	return 0;
}

/* handle game unpause*/
int setUnpause(int *choice,SDL_Event* test_event){
	int error=0;
	*choice=!(*choice);// form 1 to 0 and 0 to 1
	/* make move for relevant AI player*/
	if (cur_game->is_multiplayer==3 && cur_game->cur_player==1){
		if (cur_game->cur_player==1){
			error=cur_game->handle_computer_move( cur_game->board,cur_game->difficultyP1,cur_game->cur_player);
		}
		else {
			error=cur_game->handle_computer_move( cur_game->board,cur_game->difficultyP2,cur_game->cur_player);
		}
		if (error<0){
			printf("ERROR: could not make move after unpause");
			return -1;
		}
		/*switch players*/
		cur_game->cur_player = (-1)*(cur_game->cur_player);
		/*try to repaint GUI*/
		error=draw_game();
	}
	if (error<0){
		free(cur_game);
		gameNum--;
		freeControlList(ui_tree);
		return -1;
	}
	return 0;
}
