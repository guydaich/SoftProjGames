/* button_actions.c*/
/* This class is responsible for handeling of all on_click actions in the UI.
 * It calls window change operations (go_to_...), changes game attributes, and
 * handles the main game-play logic - detecting human moves, calling minimax
 * for AI moves, calling UI tree updates, and repaining UI */

#include "buttonActions.h"
#include "windowsDrawing.h"

#define BTN_X_OFFSET 25
#define BTN_Y_OFFSET 20
#define BTN_H 100
#define PADDING 10


int gameNum=0;

extern int quit;
extern game* cur_game;
extern element_cntrl ui_tree;

/* restarts game, according to user selection and caught sdl_event
 * returns 0 on success, -1 on failure */
int restart_game(int *choice,SDL_Event* test_event)
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
		if (strcmp(ui_tree->children->head->children->tail->cntrl->caption,QUIT)==0){
			error=new_generic_button(ui_tree->children->head->children,BTN_X_OFFSET,BTN_Y_OFFSET + BTN_H*2 + PADDING,USPAUSE,set_unpause,0);
			if (error<0){
				printf("ERROR: cannot replace pause-unpause buttons");
				return -1;
			}
			ui_tree->children->head->children->tail->parent=ui_tree->children->head;
		}
	}
	error=draw_game();
	if (error<0){
		return -1;
	}
	return 0;
}

/* ends game, according to user selection and caught sdl_event
 * by ending the main while(!quit) loop */
int quit_game(int *choice,SDL_Event* test_event)
{
	/*global that breaks SDL loops*/
	quit=1;
	return 0;
}

/* changes the current window to game selection window,
 * retuns -1 on failure*/
int go_to_game_selection(int *choice,SDL_Event* test_event)
{
	int error=0;
	error=run_window(SELECT_GAME);
	if (error<0){
		return -1;
	}
	return 0;
}

/* saves the game, according to selected save-game
 * slot. returns -1 on failure*/
int save_game(int *choice,SDL_Event* test_event)
{
	int is_rewrite=0,userAnswer=0,error=0,file_length=0;
	char* fileLocation = NULL;
	
	/*allocate file path*/
	file_length = strlen(SAVE_FILE_PREFIX) + strlen(SAVE_FILE_SUFFIX) + SAVE_FILE_MAX_DIGITS;
	if (file_length <= 0 )
	{
		printf("ERROR: illegal save file path");
		return -1; 
	}
	
	fileLocation=(char *)malloc(file_length);
	if (fileLocation == NULL)
	{
		printf("ERROR: standart function malloc has failed");
		return -1; 
	}
	sprintf(fileLocation,"%s%d%s",SAVE_FILE_PREFIX,*choice,SAVE_FILE_SUFFIX);
	
	/*check is save over-writes*/
	is_rewrite=save_game_in_file(fileLocation,cur_game->board,cur_game->cur_player,
						cur_game->cols,cur_game->rows,(cur_game->get_name()));
	if(is_rewrite==1)
	{
         //ask user if he want s to overwrite
		userAnswer=question_window("This file already exist.\nDo you wish to overwrite?\n",USER_PROMPT);
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
	else if(is_rewrite<0){
		printf("failed to write to file\n");
		free(fileLocation);
		return -1;
	}
	free(fileLocation);
	return 0;
}

/* handle the move request by game implementation: 
 * detect and handle user's move, call AI move, 
 * check for changes in the game state: victory, etc. */

int  handle_next_move(int *choice,SDL_Event* test_event)
{
	int move_success = 0,error=0;
	if (*choice==1){//paused
		return 0;
	}
	/* handle mouse click by user */
	move_success = cur_game->handle_mouse_button_down(test_event, cur_game->board, cur_game->cur_player);		
	if (move_success==0){
		return 0; 
	}
	else if(move_success<0){
		printf("ERROR: failed in handle_mouse_button_down\n");
		free(cur_game->board);
		free(cur_game);
		gameNum--;
		free_control_list(ui_tree);
		return -1;
	}
	cur_game->cur_player = (-1)*cur_game->cur_player;
	if ( !cur_game->is_game_over( cur_game->board))
	{
		error=draw_game();
		if (error<0){
			printf("ERROR: failed in draw_game\n");
			free(cur_game->board);
			free(cur_game);
			gameNum--;
			free_control_list(ui_tree);
			return -1;
		}
		SDL_Delay(100);

		/* if playing against computer */
		if (cur_game->is_multiplayer==2 || cur_game->is_multiplayer==3)
		{
			/* call game implementation of minimax wrapper, 
			 * and make AI move, according to AI player */
			if (cur_game->cur_player==1){
				error=cur_game->handle_computer_move( cur_game->board,cur_game->difficultyP1,cur_game->cur_player);
				if (error<0){
					printf("ERROR: failed in handle_computer_move\n");
					free(cur_game->board);
					free(cur_game);
					gameNum--;
					free_control_list(ui_tree);
					return -1;
				}
			}
			else {
				error=cur_game->handle_computer_move( cur_game->board,cur_game->difficultyP2,cur_game->cur_player);
				if (error<0){
					printf("ERROR: failed in handle_computer_move\n");
					free(cur_game->board);
					free(cur_game);
					gameNum--;
					free_control_list(ui_tree);
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
		printf("ERROR: failed in draw_game\n");
		free(cur_game->board);
		free(cur_game);
		gameNum--;
		free_control_list(ui_tree);
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
				free(cur_game->board);
				free(cur_game);
				gameNum--;
				free_control_list(ui_tree);
				return -1;
			}
		}
		else if (cur_game->is_victory( cur_game->board) == -1){
			error=cur_game->victoryColor(cur_game->board,-1,ui_tree);
			if (error<0){
				printf("failed in victoryColor\n");
				free(cur_game->board);
				free(cur_game);
				gameNum--;
				free_control_list(ui_tree);
				return -1;
			}
		}
		else //if tie, add to ui a tie-message
		{
			error=new_generic_button(ui_tree->children,300,480,"draw",restart_game,0);
			if (error<0){
				printf("failed in makeing draw button\n");
				free(cur_game->board);
				free(cur_game);
				gameNum--;
				free_control_list(ui_tree);
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
int  set_player1_difficulty(int *choice,SDL_Event* test_event)
{
	cur_game->difficultyP1=(cur_game->get_difficulty_levels())[*choice-1];
	if (cur_game->difficultyP1<1 || cur_game->difficultyP1>9){
		return -1;
	}
	return 0;
}

/* assign difficulty level for player 2*/
int  set_player2_difficulty(int *choice,SDL_Event* test_event)
{
	cur_game->difficultyP2=(cur_game->get_difficulty_levels())[*choice-1];
	if (cur_game->difficultyP2<1 || cur_game->difficultyP2>9){
		return -1;
	}
	return 0;
}

/* set the type of game: AI v AI, Human v AI, etc. */
int  set_game_player_types(int *choice,SDL_Event* test_event)
{
	cur_game->is_multiplayer=*choice;
	return 0;
}

/* handle game selection */
int  set_current_game(int *choice,SDL_Event* test_event)
{
	/* clear current game */
	if (cur_game!=NULL){
		free(cur_game->board);
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
int  go_to_load_menu(int *choice,SDL_Event* test_event)
{
	int error;
	//free_control_list(ui_tree);
	error=run_window(LOAD_GAME);
	if(error<0){
		return -1;
	}
	return 0;
}

/*move to the first start window*/
int  go_to_start_menu(int *choice,SDL_Event* test_event)
{
	int error;
	//free_control_list(ui_tree);
	error=run_window(SELECT_MAIN_MENU);
	if(error<0 || (cur_game==NULL && quit!=1)){
		printf("ERORR: could not move to new window");
		return -1;
	}
	else if (quit==1){
		return 0;
	}
	
	/* all these game parameters are intiilised with -1
	 * it is possible that a game is partially loaded, so,
	 * only if they are unfilled, ask for these paramters */

	if (cur_game->is_multiplayer<0)
	{
		error=run_window(SELECT_PLAYER_TYPES);
	}
	if(error<0){
		return -1;
	}
	if(cur_game->is_multiplayer<0 || cur_game->is_multiplayer>4){
		return -1;
	}
	if ((cur_game->is_multiplayer==1 || cur_game->is_multiplayer==3) && cur_game->cur_player==1){
		*choice=1; /* pauses game on start*/
	}
	else if(cur_game->is_multiplayer==2 && cur_game->cur_player==-1){
		*choice=1; /* pauses game on start*/
	}

	/* ask for player's difficulties, according to player types */
	switch(cur_game->is_multiplayer){
		case 1:
			if (cur_game->difficultyP1<0)//run DIFF1 choise only if DIFF1 wasn't chosen yet
			{
				error=run_window(SET_DIFF_PLAYER1);
			}
			if(error<0){
				return -1;
			}
			if (cur_game->difficultyP2<0)//run DIFF2 choise only if DIFF2 wasn't chosen yet
			{
				error=game_init(SET_DIFF_PLAYER2);
			}
			if(error<0){
				return -1;
			}
			break;
		case 2:
			if (cur_game->difficultyP2<0)//run DIFF2 choise only if DIFF2 wasn't chosen yet
			{
				error=game_init(SET_DIFF_PLAYER2);
			}
			if(error<0){
				return -1;
			}
			break;
		case 3:
			if (cur_game->difficultyP1<0)//run DIFF1 choise only if DIFF1 wasn't chosen yet
			{
				error=game_init(SET_DIFF_PLAYER1);
			}
			if(error<0){
				return -1;
			}
			break;
		case 4:
			error=game_init(EMPTY_WINDOW);//idea-EMPTY_WINDOW
			if(error<0){
				return -1;
			}
			break;
	}
	return 0;
}

/* load a game from save game file handle*/
int load_game(int *choice,SDL_Event* test_event)
{
	whichGame whichGame;
	int error=0,player,file_length=0;
	int *gameBoard; 
	char *fileLocation = NULL;

	/*allocate file path*/
	file_length = strlen(SAVE_FILE_PREFIX) + strlen(SAVE_FILE_SUFFIX) + SAVE_FILE_MAX_DIGITS;
	if (file_length <= 0 )
	{
		printf("ERROR: illegal save file path");
		return -1; 
	}
	
	fileLocation=(char *)malloc(file_length);
	if (fileLocation == NULL)
	{
		printf("ERROR: standart function malloc has failed");
		return -1; 
	}
	sprintf(fileLocation,"%s%d%s",SAVE_FILE_PREFIX,*choice,SAVE_FILE_SUFFIX);

	/*try loading from file*/
	error=load_game_from_file(fileLocation,&whichGame,&gameBoard,&player);
	if (error==-2){
		error = question_window("the game is either corrupt or doesn't exist\n",USER_NOTIF);
		if (error<0)
		{
			printf("ERROR: could not notify user of problem");
			return -1;
		}
		/* resume program */
		error=go_to_start_menu(choice,test_event);
		if (error<0){
			return -1;
		}
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
int go_to_save_menu(int *choice,SDL_Event* test_event)
{
	int error=0;
	//free_control_list(ui_tree);
	error=game_init(SAVE_GAME);
	if (error<0){
		return -1;
	}
	return 0;
}

/* go to difficulty Selection menu for Player 1*/
int go_to_difficulty_player1(int *choice,SDL_Event* test_event)
{
	int error=0;
	//free_control_list(ui_tree);
	error=game_init(SET_DIFF_PLAYER1);
	if (error<0){
		return -1;
	}
	return 0;
}

/* go to difficulty Selection menu for Player 2*/
int go_to_difficulty_player2(int *choice,SDL_Event* test_event)
{
	int error=0;
	//free_control_list(ui_tree);
	error=game_init(SET_DIFF_PLAYER2);
	if (error<0){
		return -1;
	}
	return 0;
}

/* handle game unpause*/
int set_unpause(int *choice,SDL_Event* test_event){
	int error=0;
	element_cntrl newTail=NULL;
	if(cur_game->is_multiplayer!=1){
		if (*choice==1){
			if (strcmp(ui_tree->children->head->children->tail->cntrl->caption,QUIT)!=0){
				newTail=ui_tree->children->head->children->tail->prev;
				free_control_list(ui_tree->children->head->children->tail);
				ui_tree->children->head->children->tail=newTail;
				newTail->next=NULL;
			}
		}
		else{
			/*over-write a button with a new button, as rightest son*/
			error=new_generic_button(ui_tree->children->head->children,BTN_X_OFFSET,BTN_Y_OFFSET + BTN_H*2 + PADDING,USPAUSE,set_unpause,0);
			if (error<0){
				printf("ERROR: cannot replace pause-unpause buttons");
				return -1;
			}
			ui_tree->children->head->children->tail->parent=ui_tree->children->head;
		}
	}
	*choice=!(*choice);// form 1 to 0 and 0 to 1
	/* make move for relevant AI player*/
	if (cur_game->is_multiplayer==3 && cur_game->cur_player==1 || cur_game->is_multiplayer==2 && cur_game->cur_player==-1){
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
	else {
		if (draw_ui_tree(ui_tree)<0)
		{
			printf("ERROR: drawing UI Tree Failed");
			return -1;
		}
		SDL_Flip( (ui_tree->cntrl->srfc) );
	}
	if (error<0){
		free(cur_game->board);
		free(cur_game);
		gameNum--;
		free_control_list(ui_tree);
		return -1;
	}
	return 0;
}
