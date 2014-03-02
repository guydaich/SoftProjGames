#include "tic_tac_toe_bl.h"



char* TIC_TAC_TOE_NAME = "Tic-Tac-Toe";
int* tic_tac_toe_board=NULL;
int tic_tac_toe_diffficulties[] = {9};

char* ttc_get_name()
{
	return TIC_TAC_TOE_NAME;
}

int* ttc_get_initial_state()
{
	int i=0,j=0;
	int* initial_board;
	if (tic_tac_toe_board!=NULL){
		free(tic_tac_toe_board);
	}
	initial_board = (int*)calloc(TIC_TAC_TOE_ROWS * TIC_TAC_TOE_COLS, sizeof(int));

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
	int* copied_state = (int*)calloc(TIC_TAC_TOE_ROWS * TIC_TAC_TOE_COLS, sizeof(int));
	int i=0,j=0;
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

linked_list ttc_get_state_children(int* game_state, int player)
{
	linked_list list = new_list();
	int i,j, return_value = 1 ;

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
			}
		}
	}
	return list; 
}


/* creates a node and element for each child-state, and adds to list*/
int ttc_add_to_children_list(linked_list list, int* game_state, int row, int col, int player)
{
	int* moved_state;
	vertex node;
	element new_elem,prev_tail;

	moved_state = ttc_copy_and_make_move(game_state,row,col,player);
	node = new_node(row*TIC_TAC_TOE_ROWS + col,(int*)moved_state,ttc_get_state_score(moved_state,player));

	/*check for errors*/
	if (node == NULL)
	{
		free(moved_state);
		return 0;
	}

	/*create new element */
	new_elem = new_element();
	
	/*handle errors*/ 
	if (new_elem == NULL)
	{
		free(moved_state);
		free(node);
		return 0;
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
	if (ttc_is_victory(game_state, player))
	{
		return INT_MAX;
	}
	else if (ttc_is_victory(game_state, (-1)*player))
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


/* non-interface functions */
void ttc_init_game()
{
	tic_tac_toe_board = ttc_get_initial_state();
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
	if (ttc_is_victory(game_state, TTC_PLAYER_1) ||
		ttc_is_victory(game_state, TTC_PLAYER_2) ||
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
int ttc_is_victory(int* game_state, int player)
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
			if (game_state[i*TIC_TAC_TOE_ROWS + i] != player)
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

element_cntrl ttc_panel_function(int* game_state,void  (*makeMove)(void* cur_game,element_cntrl* ui_tree,int *quit,SDL_Event* test_event))
{ 
	control *ttc_grid;
	control *ttc_button;
	element_cntrl root, grid, temp;
	linked_list_cntrl list;
	int i,j;
 	
	root = new_control_element(new_panel(0,0,600,600,255,255,255));
	/*create panel children*/	
	list = new_control_list();
	/* grid surface - create control and element*/
	ttc_grid = new_button(0,0,600,600,"./gfx/grid.bmp",0,0,0,0,NULL);
	grid = new_control_element(ttc_grid);
	/* add grid to children list*/
	add_control_element_to_list(list,grid);
	/* update root children, and grid parent*/
	set_list_as_children(list,root);
	
	/*create grid children*/
	list = new_control_list();
	for (i=0; i< TIC_TAC_TOE_COLS; i++)
	{
		for(j=0; j< TIC_TAC_TOE_ROWS; j++)
		{
			ttc_button= NULL;
			if (game_state[i*TIC_TAC_TOE_ROWS + j] == TTC_PLAYER_1)	
			{
				ttc_button=new_button(j*200,i*200,200,200,"./gfx/x.bmp",255,0,255,1,NULL);
			}
			else if (game_state[i*TIC_TAC_TOE_ROWS + j] == TTC_PLAYER_2)	
			{
				ttc_button=new_button(j*200,i*200,200,200,"./gfx/o.bmp",255,0,255,1,NULL);
			}
			else
			{
				ttc_button=new_button(j*200,i*200,200,200,"./gfx/ttc_empty.bmp",255,0,255,1,NULL);
				ttc_button->pressedButton=makeMove;
			}
			/*add pieces to children list*/
			if (ttc_button != NULL)
			{
				temp = new_control_element(ttc_button);
				add_control_element_to_list(list,temp);
			}
		}
	}
	/*update parent-children*/
	set_list_as_children(list,grid);

	return root;
}

int ttc_handle_mouse_button_down (SDL_Event *event,element_cntrl root, int* game_state, int player)
{
	int x=0,y=0;
	int comp_move,succes;
	element_cntrl elem=NULL;
	x=event->button.x;
	y=event->button.y;
	/* elem get's the elemnt to update (grid slot) */
	find_element_by_coordinates(root,x,y,&elem);

	if(ttc_is_game_over(game_state))
	{
		return 0;
	}
	succes=ttc_make_move(game_state,y/200,x/200,1);
	if(succes==0)
	{
		return 0;
	}
	if (ttc_is_game_over(game_state))
	{
		return 0;
	}
	return 1;
}

int	ttc_handle_computer_turn(int* game_state, int depth)
{
	int comp_move;
	comp_move = get_computer_move(game_state, 9, ttc_get_state_children);
	ttc_make_move(game_state,comp_move/TIC_TAC_TOE_ROWS,comp_move%TIC_TAC_TOE_ROWS,-1);
	return 0;
}