#include "tic_tac_toe_bl.h"

int tic_tac_toe_diffficulties[] = {9};//possible didiffficulty levels. this is a global array so it won't be freed at any point on the game.

char* ttc_get_name()
{
	return TIC_TAC_TOE_NAME;
}

/*this function makes and returns a empty ttc game board(logic, not gui) 
on failure return null*/
int* ttc_get_initial_state()
{
	int i=0,j=0;
	int* initial_board=NULL;
	initial_board = (int*)calloc(TIC_TAC_TOE_ROWS * TIC_TAC_TOE_COLS, sizeof(int));
	if(initial_board==NULL){
		printf("failed to make board(calloc) in ttc_get_initial_state\n");
		return NULL;
	}

	for (i=0; i < TIC_TAC_TOE_ROWS; i++)
	{
		for(j=0; j < TIC_TAC_TOE_COLS; j++)
		{
			initial_board[i*TIC_TAC_TOE_ROWS + j] = 0;
		}
	}
	return initial_board;
}

/*this function copys a ttc board and makes a move(according to move_col and move_row) on the copy
 returns null on failure*/
int* ttc_copy_and_make_move(int* game_state, int move_row, int move_col, int player)
{
	int i=0,j=0;
	int* copied_state = (int*)calloc(TIC_TAC_TOE_ROWS * TIC_TAC_TOE_COLS, sizeof(int));
	boardCount++;
	if (copied_state==NULL){
		printf("failed to make board(calloc) in ttc_copy_and_make_move\n");
		return NULL;
	}
	for (i=0; i< TIC_TAC_TOE_ROWS; i++)
	{
		for(j=0; j  <TIC_TAC_TOE_COLS; j++)
		{
			copied_state[i*TIC_TAC_TOE_ROWS + j] = game_state[i*TIC_TAC_TOE_ROWS + j]; 
		}
	}
	
	copied_state[move_row*TIC_TAC_TOE_ROWS + move_col] = player;

	return copied_state;
}

/*this functions returns a list of minimax elements.
 * each of these elements represents a possible move that a player can do in his turn
 * if game_state is the current board.
 * on success the function returns with *error=0, on failure returns with *error=-1*/
linked_list ttc_get_state_children(int* game_state, int player,int *error)
{
	int i,j, return_value = 1 ;
	element run_elem;
	linked_list list = new_list();
	if (list==NULL){
		*error=-1;
		return NULL;
	}

	//if game is over - no more steps
	if (ttc_is_game_over(game_state))
	{
		list->head = NULL;
		list->tail = NULL;
		return list;
	}

	for (i=0; i	< TIC_TAC_TOE_ROWS; i++)
	{
		for(j=0; j  <TIC_TAC_TOE_COLS; j++)
		{
			// if we may place at the slot i*TIC_TAC_TOE_ROWS + j on the board-add this move as a child
			if (game_state[i*TIC_TAC_TOE_ROWS + j] == 0)
			{
				return_value = ttc_add_to_children_list(list,game_state,i,j,player);
				if (return_value==-1){
					break;
				}
			}
		}
		if (return_value==-1){
			break;
		}
	}
	if (return_value==-1){//if ttc_add_to_children_list failed at any time within the for- clear list and return with failure
		for (run_elem=list->head;run_elem!=NULL && run_elem->next!=NULL;run_elem=run_elem->next){
			free(run_elem->node->game_state);
			free(run_elem->node);
			if (run_elem->prev!=NULL){
				free(run_elem->prev);
			}
		}
		//free last one
		if (run_elem!=NULL){
			free(run_elem->node->game_state);
			free(run_elem->node);
			free(run_elem);
		}
		*error=-1;
		return NULL;
	}
	*error=0;//ttc_get_state_children successeded
	return list; 
}


/* creates a minimax node and element for each child-state(game_state+move), and adds to list
on failure return null*/
int ttc_add_to_children_list(linked_list list, int* game_state, int row, int col, int player)
{
	int* moved_state=NULL;
	vertex node=NULL;
	element new_elem=NULL,prev_tail=NULL;

	moved_state = ttc_copy_and_make_move(game_state,row,col,player);
	if (moved_state==NULL){
		return -1;
	}
	node = make_node(row*TIC_TAC_TOE_ROWS + col,(int*)moved_state,ttc_get_state_score(moved_state,player));
	if (node == NULL)
	{
		free(moved_state);
		return -1;
	}

	//create new element
	new_elem = new_element();
	if (new_elem == NULL)
	{
		free(moved_state);
		free(node);
		return -1;
	}
				
	//assign node to element
	new_elem->node = node;
	// add node to list
	if (list->head == NULL)//if the list is empty- set as both head and tail
	{	
		list->head = new_elem;
		list->tail = new_elem;
		list->tail->next = NULL;
	}
	else
	{
		prev_tail =			list->tail;
		prev_tail->next =	new_elem;
		new_elem->prev =	prev_tail;
		list->tail =		new_elem;
		list->tail->next = NULL;
	}
	return 1;
}

/*this is a function which evaluate each board.
* we always work in difficulty 9(wich means we allways looks to max depth), so either win, loose or neutral. 
* the winning moves will bubble up because of the minimax*/
int ttc_get_state_score(int* game_state,int player)
{
	if (ttc_is_victory(game_state)==1)
	{
		return INT_MAX;
	}
	else if (ttc_is_victory(game_state)==-1)
	{
		return INT_MIN;
	}
	else
	{
		return 0;
	}
};

/* get difficult levels for game*/
int* ttc_get_difficulty_levels()
{
	return tic_tac_toe_diffficulties;
}

/*make a ttc move on the board of the current ttc game*/
int ttc_make_move(int* game_state, int row, int col, int player)
{
	if (game_state[row*TIC_TAC_TOE_ROWS + col] == 0)
	{
		game_state[row*TIC_TAC_TOE_ROWS + col] = player;
		return 1;
	}
	else{
		return 0;
	}
}

/*check if the game represented in game_state is over*/
int ttc_is_game_over(int* game_state)
{
	//the game is over if the board is full or somebody won
	if (ttc_is_victory(game_state)==-1 ||
		ttc_is_victory(game_state)==1 ||
		ttc_is_board_full(game_state))
			return 1;
	return 0;
}

/*check if the board represented in game_state is full */
int ttc_is_board_full(int* game_state)
{
	int i,j;
	for (i=0; i	< TIC_TAC_TOE_ROWS; i++)
	{
		for(j=0; j  <TIC_TAC_TOE_COLS; j++)
		{
			// if there is an open slot - the board isn't full
			if (game_state[i*TIC_TAC_TOE_ROWS + j] == 0)
			{
				return 0;
			}
		}
	}
	return 1;
}

/* checks if there is a victory in passed game state */
int ttc_is_victory(int* game_state){
	if (ttc_is_victory_player(game_state,1)) return 1;
	if (ttc_is_victory_player(game_state,-1)) return -1;
	return 0;
}

/*this function look at evrey row,col,diagnal and checks for 4  of the same color together(victory)*/
int ttc_is_victory_player(int* game_state, int player)
{
	int i,j,flag; 
	/* check lines */	
	
		// check rows
		for (i=0; i< TIC_TAC_TOE_ROWS; i++)
		{
			flag = 1;
			for(j=0; j< TIC_TAC_TOE_COLS; j++)
			{
				if (game_state[i*TIC_TAC_TOE_ROWS + j] != player)
				{
					flag = 0;
				}
			}
			if (flag)
			{
				return 1;
			}
		}

		// check cols
		for (i=0; i< TIC_TAC_TOE_COLS; i++)
		{
			flag = 1;
			for(j=0; j< TIC_TAC_TOE_ROWS; j++)
			{
				if (game_state[j*TIC_TAC_TOE_ROWS + i] != player)
				{
					flag = 0;
				}
			}
			if (flag)
			{
				return 1;
			}
		}

	// diagonal 1
	flag = 1;
	for (i=0; i< TIC_TAC_TOE_ROWS; i++)
		{
			
			if (game_state[i*TIC_TAC_TOE_ROWS + i] != player)
			{
				flag = 0;
			}
			
		}
		
		if (flag)
		{
			return 1;
		}

	// diagonal 2
	flag = 1;
	for (i= TIC_TAC_TOE_ROWS -1; i >=0 ; i--)
		{
			if (game_state[i*TIC_TAC_TOE_ROWS + (TIC_TAC_TOE_ROWS-i-1)] != player)
			{
				flag = 0;
			}
		}

	if (flag)
	{
		return 1;
	}
	
	return 0;
}

/*this function act as a wraper for minimax while playing ttc*/
int	ttc_handle_computer_turn(int* game_state, int depth,int player)
{
	int comp_move=0;
	//get the best move for computer with minimax
	if (player==-1){
		comp_move = get_computer_move(game_state, depth, ttc_get_state_children);
	}
	else {
		comp_move=get_suggested_move(game_state,depth, ttc_get_state_children);
	}
	if (comp_move<0){//check move validity(and for erros in minimax)
		printf("failure in ttc_handle_computer_turn(minimax)\n");
		return -1;
	}
	//make the computer move
	ttc_make_move(game_state,comp_move/TIC_TAC_TOE_ROWS,comp_move%TIC_TAC_TOE_ROWS,player);
	return 0;
}

/*this unction interpets a mouse cliking on the GUI game board into a move on the board*/
int ttc_handle_mouse_button_down (SDL_Event *event, int* game_state, int player)
{
	int x=0,y=0;
	int succes;
	x=event->button.x;
	y=event->button.y;

	if(ttc_is_game_over(game_state))//if the game is over no more moves allowed
	{
		return 0;
	}
	if (y < TTC_YOFFSET || x < TTC_XOFFSET)//check coordinates are inside ethe board
	{
		return 0;
	}
	succes=ttc_make_move(game_state,(y-TTC_YOFFSET)/TTC_HBTN,(x-TTC_XOFFSET)/TTC_WBTN,player);//make move(if possible)
	if(succes == 0)//return if the move is possible by game rules
	{
		return 0;
	}
	return 1;
}
