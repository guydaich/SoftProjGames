#include "tic_tac_toe_bl.h"
#include "ttc_ui.h"

char* TIC_TAC_TOE_NAME = "Tic-Tac-Toe";
int tic_tac_toe_diffficulties[] = {9};

char* ttc_get_name()
{
	return TIC_TAC_TOE_NAME;
}

int* ttc_get_initial_state()
{
	int i=0,j=0;
	int* initial_board;
	initial_board = (int*)calloc(TIC_TAC_TOE_ROWS * TIC_TAC_TOE_COLS, sizeof(int));
	if(initial_board==NULL){
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

int* ttc_copy_and_make_move(int* game_state, int move_row, int move_col, int player)
{
	int i=0,j=0;
	int* copied_state = (int*)calloc(TIC_TAC_TOE_ROWS * TIC_TAC_TOE_COLS, sizeof(int));
	if (copied_state==NULL){
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

linked_list ttc_get_state_children(int* game_state, int player,int *error)
{
	int i,j, return_value = 1 ;
	element run_elem;
	linked_list list = new_list();
	if (list==NULL){
		*error=-1;
		return NULL;
	}

	/*if game is over - no more steps*/
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
			/* we may place a piece in any open slot */
			if (game_state[i*TIC_TAC_TOE_ROWS + j] == 0)
			{
				/* add this move to children list*/
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
	if (return_value==-1){
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
	*error=0;
	return list; 
}


/* creates a node and element for each child-state, and adds to list*/
int ttc_add_to_children_list(linked_list list, int* game_state, int row, int col, int player)
{
	int* moved_state;
	vertex node;
	element new_elem,prev_tail;

	moved_state = ttc_copy_and_make_move(game_state,row,col,player);
	if (moved_state==NULL){
		return -1;
	}
	node = new_node(row*TIC_TAC_TOE_ROWS + col,(int*)moved_state,ttc_get_state_score(moved_state,player));

	/*check for errors*/
	if (node == NULL)
	{
		free(moved_state);
		return -1;
	}

	/*create new element */
	new_elem = new_element();
	
	/*handle errors*/ 
	if (new_elem == NULL)
	{
		free(moved_state);
		free(node);
		return -1;
	}
				
	/*assign node to element*/
	new_elem->node = node;
	/* add to list */
	if (list->head == NULL)
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

/*we always work in difficulty 9, so either win, loose or neutral. 
 the winning moves will bubble up*/
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

/* gets difficult level for game*/
int* ttc_get_difficulty_levels()
{
	return tic_tac_toe_diffficulties;
}

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

int ttc_is_game_over(int* game_state)
{
	if (ttc_is_victory(game_state)==-1 ||
		ttc_is_victory(game_state)==1 ||
		ttc_is_board_full(game_state))
			return 1;
	return 0;
}

int ttc_is_board_full(int* game_state)
{
	int i,j;
	for (i=0; i	< TIC_TAC_TOE_ROWS; i++)
	{
		for(j=0; j  <TIC_TAC_TOE_COLS; j++)
		{
			/* if there is an open slot - not full */
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


int	ttc_handle_computer_turn(int* game_state, int depth,int player)
{
	int comp_move;
	if (player==-1){
		comp_move = get_computer_move(game_state, depth, ttc_get_state_children);
	}
	else {
		comp_move=get_suggested_move(game_state,depth, ttc_get_state_children);
	}
	if (comp_move<0){
		return -1;
	}
	else if (comp_move==0){
		return -2;
	}
	ttc_make_move(game_state,comp_move/TIC_TAC_TOE_ROWS,comp_move%TIC_TAC_TOE_ROWS,player);
	return 0;
}

int ttc_handle_mouse_button_down (SDL_Event *event, int* game_state, int player)
{
	int x=0,y=0;
	int succes;
	x=event->button.x;
	y=event->button.y;
	/* elem get's the elemnt to update (grid slot) */

	if(ttc_is_game_over(game_state))
	{
		return 0;
	}
	if (y < TTC_YOFFSET || x < TTC_XOFFSET)
	{
		return 0;
	}
	succes=ttc_make_move(game_state,(y-TTC_YOFFSET)/TTC_HBTN,(x-TTC_XOFFSET)/TTC_WBTN,player);
	if(succes == 0)
	{
		return 0;
	}
	return 1;
}
