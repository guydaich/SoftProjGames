#include "reversi_bl.h"

int *reversi_board=NULL; 
int reversi_diffficulties[] = {1,2,3,4};//possible didiffficulty levels. this is a global array so it won't be freed at any point on the game.
/* all possible move directions({delta_x,delta_y}) on board*/
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

/*this function makes and returns a satrting board(logic, not gui) for reversi game
on failure return null*/
int* rv_get_initial_state()
{
	int i=0,j=0;
	int* initial_board = (int*)calloc(REVERSI_ROWS*REVERSI_COLS,sizeof(int));
	if(initial_board==NULL){
		return NULL;
	}
	
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

	node = make_node(row*REVERSI_ROWS + col,game_state,rv_get_state_score(game_state,player));
	if (node == NULL)
	{
		free(game_state);
		return -1;
	}
	return 1;
}

/* creates a minimax node and element for each child-state(game_state+move), and adds to list
on failure return null*/
int rv_add_to_children_list(linked_list list, int* game_state, int row, int col, int player)
{
	int* moved_state=NULL;
	vertex node=NULL;
	element new_elem=NULL,prev_tail=NULL;

	moved_state = rv_copy_and_make_move(game_state,row,col,player);
	if(moved_state==NULL){
		return -1;
	}
	node = make_node(row*REVERSI_ROWS + col,moved_state,rv_get_state_score(moved_state,player));
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
	// add to list
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

/*this functions returns a list of minimax elements.
 * each of these elements represents a possible move that a player can do in his turn
 * if game_state is the current board.
 * on success the function returns with *error=0, on failure returns with *error=-1*/
linked_list rv_get_state_children(int* game_state, int player,int *error)
{
	int i=0,j=0,return_value=0;
	element run_elem=NULL;
	linked_list list = new_list();
	
	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			//if we may place at the slot i*TIC_TAC_TOE_ROWS + j on the board-add this move as a child
			if (rv_is_valid_move(game_state, player,i,j))
			{
				return_value = rv_add_to_children_list(list,game_state,i,j,player);
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
	else if (list->head==NULL){//if no childern where created- this means the enemy has no moves
		if (rv_player_has_moves(game_state,(-1)*player)==1){//pass move
			rv_add_to_children_list(list,game_state,-1,-1,player);
		}
	}
	*error=0;
	return list;
}

/*this functions returns a list of minimax elements.
 * each of these elements represents a possible move that a player can do in his turn
 * if game_state is the current board.
 * on success the function returns with *error=0, on failure returns with *error=-1*/
int* rv_copy_and_make_move(int* game_state, int move_row, int move_col, int player)
{
	int i=0,j=0;
	//assign new board
	int* copied_state = (int*)calloc(REVERSI_ROWS * REVERSI_COLS, sizeof(int));
	boardCount++;
	if (copied_state==NULL){
		return NULL;
	}
	
	//copy current state
	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			copied_state[i*REVERSI_ROWS + j] = game_state[i*REVERSI_ROWS + j]; 
		}
	}
	if (move_row==-1 && move_col==-1){//pass move(return the same board-the enemy can't make any move)
		return copied_state;
	}
	//make move
	rv_make_move(copied_state,move_row,move_col,player);

	return copied_state;
}

/*return the number of pieces a player have on the board*/
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

/*this is a function which evaluate each board.
* we use this function for the minimax algorithm */
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
			else if (game_state[i*REVERSI_ROWS + j] == (-1)*player)
			{
				score -= region_scores[i][j];
			}
		}
	}
	return score;
}

/* get difficult level for game*/
int* rv_get_difficulty_levels()
{
	return reversi_diffficulties;
}

/*make a given move on the board, flips other player's pieces accordingly*/
int rv_make_move(int* game_state, int rows, int cols, int player)
{
	int i=0,t_rows=-1,t_cols=-1, length=0;
	int other = player*(-1); 

	if (!rv_is_valid_move(game_state,player,rows,cols))
	{
		return 0; 
	}

	// for every possible direction check if can flip enemys pieces
	for (i=0;i<REVERSI_ROWS; i++)
	{
		length = 0; 
		t_rows = rows + move_directions[i][0]; 
		t_cols = cols + move_directions[i][1];
		//if the derection lead as out of the board, try another 
		if (t_cols>=REVERSI_COLS || t_rows>=REVERSI_ROWS|| t_cols<0 || t_rows<0){
			continue;
		}
		while (game_state[t_rows*REVERSI_ROWS + t_cols] == other)
		{
			length++;
			
			t_rows += move_directions[i][0]; 
			t_cols += move_directions[i][1];

			// if encoutered enemy piece at that derection
			if (game_state[t_rows*REVERSI_ROWS + t_cols] == player )
			{
				//	retract moves and change colors
				while (t_rows != rows || t_cols != cols)
				{
					t_rows -= move_directions[i][0]; 
					t_cols -= move_directions[i][1];
					game_state[t_rows*REVERSI_ROWS + t_cols] = player;
				}
				// finished fliping in this direction, break
				break;
			}
		}
	}
	return 1;
}

/* a move is valid iff another piece of the player is encoutered 
   in one or more- this fuction checks if the move is valid*/ 
int rv_is_valid_move (int *game_state, int player, int rows, int cols)
{
	int i=0,t_rows=-1,t_cols=-1, length=0;
	int other = player*(-1); 
	
	//if there is another piece at the same place,can't make this move
	if (game_state[rows*REVERSI_ROWS+cols]!=0)
	{
		return 0;
	}
	// for every possible direction flip enemys pieces
	for (i=0;i<REVERSI_ROWS; i++)
	{
		length = 0; 
		t_rows = rows + move_directions[i][0]; 
		t_cols = cols + move_directions[i][1];
		
		//if the derection lead as out of the board, try another  
		if (t_cols>=REVERSI_COLS || t_rows>=REVERSI_ROWS || t_cols<0 || t_rows<0){
			continue;
		}
		// if encoutered enemy piece at that derection
		while (game_state[t_rows*REVERSI_ROWS + t_cols] == other)
		{
			length++;
			
			t_rows += move_directions[i][0]; 
			t_cols += move_directions[i][1];

			// if encoutered another piece
			if (game_state[t_rows*REVERSI_ROWS + t_cols] == player )
			{
				//	retract moves and change colors
				while (t_rows != rows || t_cols != cols)
				{
					t_rows -= move_directions[i][0]; 
					t_cols -= move_directions[i][1];
					//game_state[t_rows*REVERSI_ROWS + t_cols] = player;
				}
				// finished fliping in this direction, break
				return 1;
			}
		}
	}
	return 0;
}

/* game is over if : (1) board is full or (2) no-one can place pieces legally-this function checks if one of the condition exist*/
int rv_is_game_over(int* game_state)
{
	int i,j; 

	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			if (rv_is_valid_move(game_state,REVERSI_PLAYER_1,i,j)==1)
			{
				return 0; 
			}
			if (rv_is_valid_move(game_state,REVERSI_PLAYER_2,i,j)==1)
			{
				return 0; 
			}
		}
	}
	return 1;
}

/* checks if there is a victory in passed: return 0 for tie, 1/-1 for victory . for error return -2*/
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

/*this unction interpets a mouse cliking on the GUI game board into a move on the board*/
int rv_handle_mouse_button_down (SDL_Event *event, int* game_state,int player)
{
	int x=0,y=0;
	int succes;
	x=event->button.x;
	y=event->button.y;

	if(rv_is_game_over(game_state))
	{
		return 0;
	}
	succes=rv_make_move(game_state,(y-RVR_YOFFSET)/RVR_WBTN,(x-RVR_XOFFSET)/RVR_HBTN,player);
	if(succes==0)
	{
		if (rv_player_has_moves(game_state,player)==0){
			return 2;//pass move to next player
		}
		return 0;
	}
	return 1;
}

/*this function act as a wraper for minimax while playing reversi*/
int rv_handle_computer_turn(int* game_state, int depth,int player)
{
	int comp_move;
	if (rv_player_has_moves(game_state,player)==0){
		return 0;
	}
	do{
	if (player==-1){
		comp_move = get_computer_move(game_state, depth, rv_get_state_children);
	}
	else {
		comp_move=get_suggested_move(game_state,depth, rv_get_state_children);
	}
	if (comp_move<0){
		return -1;
	}
	if (!rv_is_valid_move(game_state,player,comp_move/REVERSI_ROWS,comp_move%REVERSI_COLS)){
		return -2;//if the minimax algorithm return unvalid move,an error accord
	}
	rv_make_move(game_state,comp_move/REVERSI_ROWS,comp_move%REVERSI_COLS,player);
	} while (rv_player_has_moves(game_state,-1*player)==0 && rv_player_has_moves(game_state,player)==1);
	return 0;
}

/*check if given player can make a move. if cam returns 1,else 0.*/
int rv_player_has_moves(int* game_state, int player)
{
	int i,j;

	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			if (rv_is_valid_move(game_state, player,i,j))
			{
	            return 1;
            }
		}
	}
	return 0;
}