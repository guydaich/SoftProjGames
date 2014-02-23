#include "reversi_bl.h"

int *reversi_board; 
int reversi_diffficulties[] = {1,2,3,4};
/* all possible {delta_x,delta_y} directions on board*/
int move_directions[8][2] = {{1,1},{-1,-1},{1,-1},{-1,1},{0,1},{1,0},{-1,0},{0,-1}};
int region_scores[REVERSI_ROWS][REVERSI_COLS] = 
				{	{10, -5, 5, 5, 5, 5, -5, 10},
					{-5, -5, -1, -1, -1, -1, -5, -5},
					{ 5, -1, 1, 1, 1, 1, -1, 5},
					{ 5, -1, 1, 1, 1, 1, -1, 5},
					{ 5, -1, 1, 1, 1, 1, -1, 5},
					{ 5, -1, 1, 1, 1, 1, -1, 5},
					{-5, -5, -1, -1, -1, -1, -5, -5},
					{10, -5, 5, 5, 5, 5, -5, 10}	};

char* rv_get_name()
{
	return REVERSI_NAME;
}

int* rv_get_initial_state()
{
	int* initial_board = (int*)calloc(REVERSI_ROWS*REVERSI_COLS,sizeof(int));
	
	int i=0,j=0;
	for (i=0; i< REVERSI_ROWS; i++)
	{
		for(j=0; j  <REVERSI_COLS; j++)
		{
			initial_board[i*REVERSI_ROWS + j] = 0; 
		}
	}
	
	// initial pieces
	initial_board[3*REVERSI_ROWS + 3] = 1;
	initial_board[4*REVERSI_ROWS + 3] = 1;
	initial_board[4*REVERSI_ROWS + 4] = -1;
	initial_board[3*REVERSI_ROWS + 4] = -1;

	return initial_board;
}

/* creates a node and element for each child-state, and adds to list*/
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
			if (is_valid_move(game_state, player,i,j))
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
void rv_make_move(int* game_state, int rows, int cols, int player)
{
	int i,t_rows,t_cols, length;
	int other = player*(-1); 

	if (!is_valid_move(game_state,player,rows,cols))
	{
		return; 
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
}

/* a move is valid iff another piece of the player is encoutered 
   in one or more*/ 
int is_valid_move (int *game_state, int player, int rows, int cols)
{
	int i,t_rows,t_cols, length;
	int other = player*(-1); 
	/* for every possible direction */
	for (i=0;i<REVERSI_ROWS; i++)
	{
		t_rows = rows + move_directions[i][0]; 
		t_cols = cols + move_directions[i][1];

		/* advance in this direction as long as enemy pieces are encountered */
		while (game_state[t_rows*REVERSI_ROWS + t_cols] == other)
		{
			t_rows += move_directions[i][0]; 
			t_cols += move_directions[i][1];	
			/* if encoutered another piece - move is legal */
			if (game_state[t_rows*REVERSI_ROWS + t_cols] == player)
			{
				return 1;
			}
		}
	}
	/*didn't encouter enemy piece - move illegal*/
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
			if (is_valid_move(game_state,REVERSI_PLAYER_1,i,j))
			{
				return 0; 
			}
			if (is_valid_move(game_state,REVERSI_PLAYER_2,i,j))
			{
				return 0; 
			}
		}
	}
	return 1;
}

/* checks if there is a victory in passed game state */
/*int rv_is_victory(int* game_state, int player)
{

}*/

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

