/* general_game.c
*  this serves a a general game class. it is instantiated when a game is chosen	
*  (directly, loaded, etc.), with an appropriarte game. it's members and methods 
*  are updated with game-specific values, and pointers to game-specific function
*  implementations 
*/
#include "general_game.h"
#include "game.h"

int gameNum=0;

extern int quit;

/* inits a new game */
game* new_game(int game_id)
{
	//int is_multi; 

	/*TODO: handle multiplayer selection
	*
	*
	*
	*/
	//is_multi = 0; // play against minimax

	game *new_game_obj = (game*)malloc(sizeof(game));  //TODO check new_game_obj!=NULL
	gameNum++;
	switch (game_id)
	{
	case TTC:
		new_game_obj->get_name = ttc_get_name;
		new_game_obj->get_state_children = ttc_get_state_children;
		new_game_obj->get_difficulty_levels = ttc_get_difficulty_levels;
		new_game_obj->get_state_score = ttc_get_state_score;
		new_game_obj->panel_function = ttc_panel_function;
		new_game_obj->get_initial_state = ttc_get_initial_state;
		new_game_obj->rows =  TIC_TAC_TOE_ROWS;
		new_game_obj->cols = TIC_TAC_TOE_COLS;
		new_game_obj->cur_player= TTC_PLAYER_1; 
		new_game_obj->handle_mouse_button_down= ttc_handle_mouse_button_down;
		new_game_obj->handle_computer_move=ttc_handle_computer_turn;
		new_game_obj->is_game_over=ttc_is_game_over;
		new_game_obj->is_multiplayer = 0;
		new_game_obj->is_victory = ttc_is_victory;
		new_game_obj->player_has_moves = rv_player_has_moves;
		new_game_obj->difficulty_num=1;
		break;
	case REVERSI:
		new_game_obj->get_name = rv_get_name;
		new_game_obj->get_state_children = rv_get_state_children;
		new_game_obj->get_difficulty_levels = rv_get_difficulty_levels;
		new_game_obj->get_state_score = rv_get_state_score;
		new_game_obj->panel_function = rv_panel_function;
		new_game_obj->get_initial_state = rv_get_initial_state;
		new_game_obj->rows =  REVERSI_ROWS;
		new_game_obj->cols = REVERSI_COLS;
		new_game_obj->cur_player= REVERSI_PLAYER_1;
		new_game_obj->handle_mouse_button_down= rv_handle_mouse_button_down;
		new_game_obj->handle_computer_move=rv_handle_computer_turn;
		new_game_obj->is_game_over=rv_is_game_over;
		new_game_obj->is_multiplayer = 0;
		new_game_obj->is_victory = rv_is_victory;
		new_game_obj->player_has_moves = rv_player_has_moves;
		new_game_obj->difficulty_num=4;
		break;
	case CONNECT4:
		new_game_obj->get_name = get_name_C4;
		new_game_obj->get_state_children = get_state_children_C4;
		new_game_obj->get_difficulty_levels = get_difficulty_levels_C4;
		new_game_obj->get_state_score = get_state_score_C4;
		new_game_obj->panel_function = C4_panel_function;
		new_game_obj->get_initial_state = get_initial_state_C4;
		new_game_obj->rows =  CONNECT4_ROWS;
		new_game_obj->cols = CONNECT4_COLS;
		new_game_obj->cur_player= CONNECT4_PLAYER_1;
		new_game_obj->handle_mouse_button_down= C4_handle_mouse_button_down;
		new_game_obj->handle_computer_move=C4_handle_computer_turn;
		new_game_obj->is_game_over=is_game_over_C4;
		new_game_obj->is_multiplayer = 0;
		new_game_obj->is_victory = is_victory_C4;
		new_game_obj->difficulty_num=7;
		break;
	default:
		break;
	}

	new_game_obj->board = new_game_obj->get_initial_state();
	new_game_obj->difficulty=new_game_obj->get_difficulty_levels()[0];

return new_game_obj;

}

void  restartGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	(*cur_game)->board=(*cur_game)->get_initial_state();
	(*cur_game)->cur_player=1;
	if ((*cur_game)->is_multiplayer==3){
		*choice=1;
	}
	(*ui_tree)=draw_game(*cur_game,*ui_tree);
}

void  quitGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	quit=1;
}

void  goToMainMenu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	freeControlList(*ui_tree);
	if ((*cur_game)!=NULL)
	{
		free((*cur_game));
		gameNum--;
	}
	(*cur_game)=runWindow(MAIN_SIGN,cur_game);
	(*ui_tree)=NULL;
}

void  saveGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	char* fileLocation=(char *)malloc(36);
	sprintf(fileLocation,"C:/Users/David/Documents/load%d.txt",*choice);
	save_game_to_file(fileLocation,(*cur_game)->board,(*cur_game)->cur_player,
						(*cur_game)->cols,(*cur_game)->rows,((*cur_game)->get_name()));
}

void  makeMove(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	element_cntrl temp_elem;
	int move_success = 0;
	if (*choice==1){
		return;
	}
	move_success = (*cur_game)->handle_mouse_button_down(test_event, (*ui_tree), (*cur_game)->board, (*cur_game)->cur_player);		
	if (!move_success)
		return; 
	//(*ui_tree)=draw_game( (*cur_game),(*ui_tree));
	//SDL_Delay( 1000 );
	(*cur_game)->cur_player = (-1)*(*cur_game)->cur_player;
	if ( !(*cur_game)->is_game_over( (*cur_game)->board)){
		if ((*cur_game)->is_multiplayer==2 || (*cur_game)->is_multiplayer==3){// playing against computer
			(*cur_game)->handle_computer_move( (*cur_game)->board,(*cur_game)->difficulty,(*cur_game)->cur_player);
			(*cur_game)->cur_player = (-1)*  (*cur_game)->cur_player;
		}	
	}
	(*ui_tree)=draw_game( (*cur_game),(*ui_tree));

	if ( (*cur_game)->is_game_over( (*cur_game)->board)){
		if ((*cur_game)->is_victory( (*cur_game)->board,1) == 1){
			newButtonGeneric((*ui_tree)->children,300,480,"victoryOne",restartGame,0);
			(*ui_tree)->children->tail->parent=(*ui_tree);//temp_elem->parent=(*ui_tree);
			temp_elem=(*ui_tree)->children->tail;
		}
		else if ((*cur_game)->is_victory( (*cur_game)->board,-1) == 1){
			newButtonGeneric((*ui_tree)->children,300,480,"victoryTwo",restartGame,0);
			(*ui_tree)->children->tail->parent=(*ui_tree);//temp_elem->parent=(*ui_tree);
			temp_elem=(*ui_tree)->children->tail;
		}
		else
		{
			newButtonGeneric((*ui_tree)->children,300,480,"draw",restartGame,0);
			(*ui_tree)->children->tail->parent=(*ui_tree);//temp_elem->parent=(*ui_tree);
			temp_elem=(*ui_tree)->children->tail;
		}
		//TODO: handle game over
		draw_button(temp_elem->cntrl,temp_elem->parent->cntrl);
		SDL_Flip((*ui_tree)->cntrl->srfc);
	}
	return;
}

void  setDifficalty(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	(*cur_game)->difficulty=((*cur_game)->get_difficulty_levels())[*choice-1];
}

void  setmultiplayer(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	(*cur_game)->is_multiplayer=*choice;
}

void  chooseGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	(*cur_game)=new_game(*choice);
	(*cur_game)->board=(*cur_game)->get_initial_state();
}

void  runLoadManu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	freeControlList(*ui_tree);
	if ((*cur_game)!=NULL)
	{
		free((*cur_game));
		gameNum--;
	}
	(*cur_game)=runWindow(LOAD_SIGN,cur_game);
	(*ui_tree)=NULL;//important
}

void  runStartManu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	freeControlList(*ui_tree);
	if ((*cur_game)!=NULL)
	{
		free((*cur_game));
		gameNum--;
	}
	(*cur_game)=runWindow(START_SIGN,cur_game);
	(*cur_game)=runWindow(AI_SIGN,cur_game);
	if ((*cur_game)->is_multiplayer==1 || (*cur_game)->is_multiplayer==3){
		*choice=1;
	}
	(*ui_tree)=game_init(cur_game,DIFF_SIGN);//important
}

void  loadGame(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
int whichGame;
	int *gameBoard; 
	char* fileLocation=(char *)malloc(36);
	sprintf(fileLocation,"C:/Users/David/Documents/load%d.txt",*choice);
	gameBoard=load_game_from_file(fileLocation,&whichGame);
	free(fileLocation);
	(*cur_game)=new_game(whichGame);
	(*cur_game)->board=gameBoard;
}

void  runsaveManu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	freeControlList(*ui_tree);
	(*ui_tree)=game_init(cur_game,SAVE_SIGN);
}

void  runDiffManu(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	freeControlList(*ui_tree);
	(*ui_tree)=game_init(cur_game,DIFF_SIGN);
}

void  setUnpause(game** cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event){
	*choice=!(*choice);// form 1 to 0 and 0 to 1
	if ((*cur_game)->is_multiplayer==3 && (*cur_game)->cur_player==1){
		(*cur_game)->handle_computer_move((*cur_game)->board,(*cur_game)->difficulty,(*cur_game)->cur_player);
		(*cur_game)->cur_player = (-1)*(*cur_game)->cur_player;
	}
	(*ui_tree)=draw_game(*cur_game,*ui_tree);
}
