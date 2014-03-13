#include "reversi_bl.h"

int *reversi_board; 
int reversi_diffficulties[] = {1,2,3,4};
/* all possible {delta_x,delta_y} directions on board*/
int move_directions[8][2] = {{1,1},{-1,-1},{1,-1},{-1,1},{0,1},{1,0},{-1,0},{0,-1}};
/*region score matrix*/
int region_scores[REVERSI_ROWS][REVERSI_COLS] = 
				{{R5, R4, R3, R3, R3, R3, R4, R5},
				{R4, R4, R2, R2, R2, R2, R4, R4},
				{R3, R2, R1, R1, R1, R1, R2, R3},
				{R3, R2, R1, R1, R1, R1, R2, R3},
				{R3, R2, R1, R1, R1, R1, R2, R3},
				{R3, R2, R1, R1, R1, R1, R2, R3},
				{R4, R4, R2, R2, R2, R2, R4, R4},
				{R5, R4, R3, R3, R3, R3, R4, R5}};


char* rv_get_name()
{
	return REVERSI_NAME;
}

int* rv_get_initial_state()
{
	int* initial_board = (int*)calloc(REVERSI_ROWS*REVERSI_COLS,sizeof(int));
	
	int i=0,j=0;
	for (i=0; i< REVERSI_ROWS; i++){
		for(j=0; j  <REVERSI_COLS; j++){
			initial_board[i*REVERSI_ROWS + j] = REVERSI_NO_PLAYER; 
		}
	}
	
	/* initial piece layout */
	initial_board[PLAYER_1_START_1] = REVERSI_PLAYER_1;
	initial_board[PLAYER_2_START_1] = REVERSI_PLAYER_2;
	initial_board[PLAYER_1_START_2] = REVERSI_PLAYER_1;
	initial_board[PLAYER_2_START_2] = REVERSI_PLAYER_2;

	return initial_board;
}

/* creates a node and element for each child-state, and adds to list*/
int rv_make_node(int* game_state, int row, int col, int player)
{
	int* moved_state;
	vertex node;
	element new_elem,prev_tail;

	moved_state = rv_copy_and_make_move(game_state,row,col,player);
	node = new_node(row*REVERSI_ROWS + col,moved_state,rv_get_state_score(moved_state,player));

	/*check for errors*/
	if (node == NULL)
	{
		free(moved_state);
		return 0;
	}
}

int rv_add_to_children_list(linked_list list, int* game_state, int row, int col, int player)
{
	int* moved_state;
	vertex node;
	element new_elem,prev_tail;

	moved_state = rv_copy_and_make_move(game_state,row,col,player);
	node = new_node(row*REVERSI_ROWS + col,moved_state,rv_get_state_score(moved_state,player));

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
	}
	else
	{
		prev_tail =			list->tail;
		prev_tail->next =	new_elem;
		new_elem->prev =	prev_tail;
		list->tail =		new_elem;
	}
	return 1;
}


linked_list rv_get_state_children(int* game_state, int player)
{
	int i,j,return_value;
	linked_list list = new_list();
	
	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			/*if possible to place piece in this position*/
			if (rv_is_valid_move(game_state, player,i,j))
			{
				/*add move to list*/
				return_value = rv_add_to_children_list(list,game_state,i,j,player);	
			}
		}
	}

	// no steps left - game is over, pass empty list
	if (list->head == NULL)
	{
		list->head = NULL;
		list->tail = NULL;
	}
	
	return list;
}

int* rv_copy_and_make_move(int* game_state, int move_row, int move_col, int player)
{
	int i=0,j=0;
	/*assign new board*/
	int* copied_state = (int*)calloc(REVERSI_ROWS * REVERSI_COLS, sizeof(int));
	
	/*copy current state*/
	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			copied_state[i*REVERSI_ROWS + j] = game_state[i*REVERSI_ROWS + j]; 
		}
	}
	
	/*make new move - turning enemy pieces as you go*/
	rv_make_move(copied_state,move_row,move_col,player);

	return copied_state;
}

int get_player_pieces(int* game_state, int player)
{
	int i,j,count=0;
	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			if (game_state[i*REVERSI_ROWS +j] == player)
			{
				count++;
			}
		}
	}
	return count;
}

/* returns score state, (+) for PLAYER_1, (-) for PLAYER_2 */
int rv_get_state_score(int* game_state,int player)
{
	int i=0,j = 0, player_pieces =0, other_pieces = 0; 
	int score=0; 

	if (rv_is_game_over(game_state))
	{
		player_pieces = get_player_pieces(game_state,player);
		other_pieces = get_player_pieces(game_state, (-1) * player);
		if (player_pieces > other_pieces)	
		{
			return INT_MAX;
		}
		else if (player_pieces < other_pieces)
		{
			return INT_MIN;
		}
		else // tie
		{
			return 0 ;
		}
	}

	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			/* if position is empty: multiply by zero, else, (+1)/(-1) according to piece*/ 
			/*score += game_state[i*REVERSI_ROWS + j] * region_scores[i][j]; */
			if (game_state[i*REVERSI_ROWS + j] == player)
			{
				score += region_scores[i][j];
			}
		}
	}
	
	if (player=REVERSI_PLAYER_2)
	{
		return (-1)* score;
	}
	else 
	{
		return score;
	}
}

int* rv_get_difficulty_levels()
{
	return reversi_diffficulties;
}


/* non-interface functions */
void rv_init_game()
{
	reversi_board = rv_get_initial_state();
}

/*get a position, flips other player's pieces*/
int rv_make_move(int* game_state, int rows, int cols, int player)
{
	int i,t_rows,t_cols, length;
	int other = player*(-1); 

	if (!rv_is_valid_move(game_state,player,rows,cols))
	{
		return 0; 
	}

	/* for every possible direction */
	for (i=0;i<REVERSI_ROWS; i++)
	{
		length = 0; 
		t_rows = rows + move_directions[i][0]; 
		t_cols = cols + move_directions[i][1];

		while (game_state[t_rows*REVERSI_ROWS + t_cols] == other)
		{
			length++;
			
			t_rows += move_directions[i][0]; 
			t_cols += move_directions[i][1];

			/* encoutered another piece */
			if (game_state[t_rows*REVERSI_ROWS + t_cols] == player )
			{
				/*	retract moves and change colors */
				while (t_rows != rows || t_cols != cols)
				{
					t_rows -= move_directions[i][0]; 
					t_cols -= move_directions[i][1];
					game_state[t_rows*REVERSI_ROWS + t_cols] = player;
				}
				/* finished in this direction, break*/
				break;
			}
		}
	}
	return 1;
}

/* a move is valid iff another piece of the player is encoutered 
   in one or more*/ 
int rv_is_valid_move (int *game_state, int player, int rows, int cols)
{
	int i,t_rows,t_cols, length,j;
	int other = player*(-1); 

	if (game_state[rows*REVERSI_ROWS+cols]!=0)
	{
		return 0;
	}
	/*
	// for every possible direction 
	for (i=0;i<REVERSI_ROWS; i++)
	{
		t_rows = rows + move_directions[i][0]; 
		t_cols = cols + move_directions[i][1];

		// advance in this direction as long as enemy pieces are encountered 
		while (game_state[t_rows*REVERSI_ROWS + t_cols] == other)
		{
			t_rows += move_directions[i][0]; 
			t_cols += move_directions[i][1];	
			// if encoutered another piece - move is legal 
			if (game_state[t_rows*REVERSI_ROWS + t_cols] == player)
			{
				return 1;
			}
		}
	}
	//didn't encouter enemy piece - move illegal
	return 0;
	*/

	//check row up
	if (rows!=0 && game_state[(rows-1)*REVERSI_COLS + cols]==other)
	{
		for (i=rows-1; i>=0; i--)
		{
			if (game_state[(i)*REVERSI_COLS + cols]==player)
			{
				return 1;
			}
		}
	}
	//check row down
	if (rows!=REVERSI_ROWS-1 && game_state[(rows+1)*REVERSI_COLS + cols]==other)
	{
		for (i=rows+1; i<REVERSI_ROWS; i++)
		{
			if (game_state[(i)*REVERSI_COLS + cols]==player)
			{
				return 1;
			}
		}
	}
	//check colnum right 
	if (cols!=REVERSI_COLS-1 && game_state[(rows)*REVERSI_COLS + cols+1]==other)
	{
		for (i=cols+1; i<REVERSI_COLS; i++)
		{
			if (game_state[(rows)*REVERSI_COLS + i]==player)
			{
				return 1;
			}
		}
	}
	//check colnum left
	if (cols!=0 && game_state[(rows)*REVERSI_COLS + cols-1]==other)
	{
		for (i=cols-1; i>=0; i--)
		{
			if (game_state[(rows)*REVERSI_COLS + i]==player)
			{
				return 1;
			}
		}
	}
	//check diaganal down right
	if (cols!=REVERSI_COLS-1 && rows!=REVERSI_ROWS-1 && game_state[(rows+1)*REVERSI_COLS + cols+1]==other)
	{
		for (i=1; rows+i<REVERSI_ROWS && cols+i<REVERSI_COLS; i++)
		{
			if (game_state[(rows+i)*REVERSI_COLS + cols+i]==player)
			{
				return 1;
			}
		}
	}
	//check diaganal up right
	if (cols!=REVERSI_COLS-1 && rows!=0 && game_state[(rows+1)*REVERSI_COLS + cols-1]==other)
	{
		for (i=1; rows-i>=0 && cols+i<REVERSI_COLS; i++)
		{
			if (game_state[(rows-i)*REVERSI_COLS + cols+i]==player)
			{
				return 1;
			}
		}
	}
	//check diaganal up left
	if (cols!=0 && rows!=0 && game_state[(rows-1)*REVERSI_COLS + cols-1]==other)
	{
		for (i=1; rows-i>=0 && cols-i>=0; i++)
		{
			if (game_state[(rows-i)*REVERSI_COLS + cols-i]==player)
			{
				return 1;
			}
		}
	}
	//check diaganal up left
	if (cols!=0 && rows!=REVERSI_ROWS-1 && game_state[(rows+1)*REVERSI_COLS + cols-1]==other)
	{
		for (i=1; rows-i<REVERSI_ROWS && cols-i>=0; i++)
		{
			if (game_state[(rows+i)*REVERSI_COLS + cols-i]==player)
			{
				return 1;
			}
		}
	}
	return 0;
}

/* game is over if : (1) board is full or (2) no-one can place pieces legally*/
int rv_is_game_over(int* game_state)
{
	int i,j; 

	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			if (rv_is_valid_move(game_state,REVERSI_PLAYER_1,i,j))
			{
				return 0; 
			}
			if (rv_is_valid_move(game_state,REVERSI_PLAYER_2,i,j))
			{
				return 0; 
			}
		}
	}
	return 1;
}

/* checks if there is a victory in passed: return 0 for tie, 1/-1 for victor */
int rv_is_victory(int* game_state)
{
	int player_pieces,other_pieces;

	if (rv_is_game_over(game_state))
	{
		player_pieces = get_player_pieces(game_state,REVERSI_PLAYER_1);
		other_pieces = get_player_pieces(game_state,REVERSI_PLAYER_2);
		if (player_pieces > other_pieces)	
		{
			return REVERSI_PLAYER_1;
		}
		else if (player_pieces < other_pieces)
		{
			return REVERSI_PLAYER_2;
		}
		else // tie
		{
			return 0 ;
		}
	}
	return -2;
}


element_cntrl	rv_panel_function(int* game_state,void  (*makeMove)(void* cur_game,element_cntrl* ui_tree,int *quit,SDL_Event* test_event))
{
	control *rv_grid;
	control *rv_button;
	element_cntrl root, grid, temp;
	linked_list_cntrl list;
	int i,j;
 	
	root = new_control_element(new_panel(0,0,600,600,255,255,255));
	/*create panel children*/	
	list = new_control_list();
	/* grid surface - create control and element*/
	rv_grid = new_button(0,0,600,600,"./gfx/reversi_board.bmp",0,0,0,0,NULL);
	grid = new_control_element(rv_grid);
	/* add grid to children list*/
	add_control_element_to_list(list,grid);
	/* update root children, and grid parent*/
	set_list_as_children(list,root);
	
	/*create grid children*/
	list = new_control_list();
	for (i=0; i< REVERSI_ROWS; i++)
	{
		for(j=0; j< REVERSI_COLS; j++)
		{
			rv_button= NULL;
			if (game_state[i*REVERSI_ROWS + j] == REVERSI_PLAYER_1)	
			{
				rv_button=new_button(50+(j)*75,50+(i)*75,75,75,"./gfx/reversi_piece_black.bmp",255,0,255,1,NULL);
			}
			else if (game_state[i*REVERSI_ROWS + j] == REVERSI_PLAYER_2)	
			{
				rv_button=new_button(50+(j)*75,50+(i)*75,75,75,"./gfx/reversi_piece_white.bmp",255,0,255,1,NULL);
			}
			else
			{
				rv_button=new_button(50+(j)*75,50+(i)*75,75,75,"./gfx/ttc_empty.bmp",255,0,255,1,NULL);
				rv_button->pressedButton=makeMove;
			}
			/*add pieces to children list*/
			if (rv_button != NULL)
			{
				temp = new_control_element(rv_button);
				add_control_element_to_list(list,temp);
			}
		}
	}
	/*update parent-children*/
	set_list_as_children(list,grid);

	return root;
}

int rv_handle_mouse_button_down (SDL_Event *event,element_cntrl root, int* game_state,int player)
{
	int x=0,y=0;
	int succes;
	element_cntrl elem=NULL;
	x=event->button.x;
	y=event->button.y;

	if(rv_is_game_over(game_state))
	{
		return 0;
	}
	succes=rv_make_move(game_state,(y-50)/75,(x-50)/75,player);
	if(succes==0)
	{
		return 0;
	}
	return 1;
}

int rv_handle_computer_turn(int* game_state, int depth)
{
	int comp_move;
	comp_move = get_computer_move(game_state, 2, rv_get_state_children);
	if (!rv_is_valid_move(game_state,-1,comp_move/REVERSI_ROWS,comp_move%REVERSI_COLS)){
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		comp_move=-1;
	}
	rv_make_move(game_state,comp_move/REVERSI_ROWS,comp_move%REVERSI_COLS,-1);
	return 0;
}

/*check if player can make another move*/
int rv_player_has_moves(int* game_state, int player)
{
	linked_list children = rv_get_state_children(game_state,player);
	if (children->head == NULL)
		return 0; 
	return 1;
}