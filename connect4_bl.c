/*						io.c									*/
/* module handles game main loop: iterates until game is over,	*/
/* reacts to user commands, and calls for computer moves		*/
#include "connect4_bl.h"

char* Connect4_NAME = "Connect4";
int* game_matrix;
int Connect4_diffficulties[] = {1,2,3,4,5,6,7};
int max_col_heights[GCOLS];

/* adds a piece in top of given Column, for stated player)*/
void add_piece_to_board_C4(int* game_state,int column, int player)
{
	/* set Human piece at top of selected column */
	max_col_heights[column-1]--;
	/* update player pieces*/
	game_state[max_col_heights[column - 1]*GCOLS +column - 1] = player;
}


/* initializes a game matrix */
int *get_initial_state_C4()
{
	int i,j = 0;
	game_matrix=(int *)calloc(GROWS*GCOLS,sizeof(int));
	for (i =0; i < GROWS ; i++)
	{
		for (j=0; j< GROWS; j++)
		{
			(game_matrix)[i*GCOLS+j] = 0; 
		}
	}
	init_col_heights_C4();
	return game_matrix;
}

/* sets all columns to empty */ 
void init_col_heights_C4()
{
int i =0;

for (i=0; i<GCOLS; i++)
	{
		max_col_heights[i] = GROWS;
	}
}

/*returns 1 if board is full, 0 otherwise*/
int is_board_full_C4()
{
int i =0;

for (i=0; i<GCOLS; i++)
	{
		/* if exists a non-full column, board not full */
		if ((max_col_heights[i]) != 0)
		{
			return 0;
		}
	}
return 1;
}

/* updates game to reflect victory*/
void victory_C4(int winner,int *game_over)
{
	*game_over = 1;
	if (winner==COMPUTER)
	{
		printf("Game over: computer wins\n");
	}
	else
	{
		printf("Game over: you win\n");
	}
}


// Calculates Scoring for a Board
int get_state_score_C4(int* game_matrix,int player)	//asume game_matrix=2D array[GROWS][GCOLS]
{
	int sizesOFLine[8] = { 0 };
	int weightVector[6] = { -5, -2, -1, 1, 2, 5 };
	int i, j, k, lineScore = 0, totalScore = 0;

	//lines in rows calculation
	for (i = 0; i < GROWS; i++) {
		for (j = 0; j < GCOLS - 3; j++) {
			lineScore = 0;
			for (k = 0; k < 4; k++) {
				if (game_matrix[(i)*GCOLS + j + k] == HUMAN){
					//GCOLS and not rows because move next line==move on all colunms of perv
					lineScore++;
				}
				else if (game_matrix[(i)*GCOLS + j + k] == COMPUTER){
					lineScore--;
				}
			}
			if (lineScore == 0){
				continue;
			}
			else if (lineScore > 0) {
				sizesOFLine[lineScore + 3]++;
			}
			else{
				sizesOFLine[lineScore + 4]++;
			}
		}
	}

	//lines in clonums calculation
	for (j = 0; j < GCOLS; j++) {
		for (i = 0; i < GROWS - 3; i++) {
			lineScore = 0;
			for (k = 0; k < 4; k++) {
				if (game_matrix[(i + k)*GCOLS + j] == HUMAN){//is it really HUMAN????
					lineScore++;
				}
				else if (game_matrix[(i + k)*GCOLS + j] == COMPUTER){
					lineScore--;
				}
			}
			if (lineScore == 0){
				continue;
			}
			else if (lineScore > 0) {
				sizesOFLine[lineScore + 3]++;
			}
			else{
				sizesOFLine[lineScore + 4]++;
			}
		}
	}

	//lines in diaganals calculation
	for (j = 0; j < GCOLS; j++) {
		for (i = 0; i < GROWS; i++) {
			lineScore = 0;
			if (((i + 3) < GROWS) && ((j + 3) < GCOLS)){//diganals which go up and right
				for (k = 0; k < 4; k++) {
					if (game_matrix[(i + k)*GCOLS + j + k] == HUMAN){
						lineScore++;
					}
					else if (game_matrix[(i + k)*GCOLS + j + k] == COMPUTER){
						lineScore--;
					}
				}
				if (lineScore == 0){
					continue;
				}
				else if (lineScore > 0) {
					sizesOFLine[lineScore + 3]++;
				}
				else{
					sizesOFLine[lineScore + 4]++;
				}
			}
			lineScore = 0;
			if (((i - 3) > -1) && ((j + 3) < GCOLS)){//diganals which go down and right
				for (k = 0; k < 4; k++) {
					if (game_matrix[(i - k)*GCOLS + j + k] == HUMAN){
						lineScore++;
					}
					else if (game_matrix[(i - k)*GCOLS + j + k] == COMPUTER){
						lineScore--;
					}
				}
				if (lineScore == 0){
					continue;
				}
				else if (lineScore > 0) {
					sizesOFLine[lineScore + 3]++;
				}
				else{
					sizesOFLine[lineScore + 4]++;
				}
			}
		}
	}

	if (sizesOFLine[0] > 0){
		return INT_MIN; //COMPUTER won
	}
	if (sizesOFLine[7] > 0){
		return INT_MAX; //HUMAN won
	}

	for (i = 1; i < 7; i++){
		totalScore = totalScore + sizesOFLine[i] * weightVector[i - 1];
	}
	return totalScore;
}

//make a copy of the board
int * copy_and_make_move_C4(board_t from,int move_row, int move_col, int player) 
{
	int i, j;
	int *new_board_ptr = // allocate new board
		(int*)calloc(GROWS*GCOLS, sizeof(int));	
	boardCount++;
	
	// handle calloc error
	if (new_board_ptr == NULL)
	{
		perror("Error: standard function calloc has failed");
		return NULL;
	}
	for (i = 0; i < GROWS; i++)		// copy board
	{
		for (j = 0; j < GCOLS; j++)
		{
			new_board_ptr[i*GCOLS + j] = (from)[i*GCOLS + j];
		}
	}
	
	for (i = GROWS - 1; i >= 0; i--) // find appropriate row for column move
	{
		if (new_board_ptr[i*GCOLS + move_col] == 0)
		{
			new_board_ptr[i*GCOLS + move_col] = player;
			break;
		}
	}
	return new_board_ptr;
}

// adds children to a node in the process of building the tree
linked_list get_state_children_C4(int* matrix, int player)
{
	linked_list newList;
	int move;
	int child_player = player;		// player is opposite

	newList=new_list();

	if (get_state_score_C4(matrix,0) == INT_MAX ||
		get_state_score_C4(matrix,0) == INT_MIN)		//if victory achieved stop building
	{
		return newList;							//return
	}

	for (move = 1; move < 8; move++)
	{
		if ((matrix)[move - 1] != 0)	// if game doesn't allow this move
		{
			continue;								// continue to next move
		}
		add_to_children_list_C4( newList,matrix,0,move,child_player);
	}
	return newList;
}


int add_to_children_list_C4(linked_list list, int* game_state, int row, int col, int player)
{
	int move=col;
	int* moved;
	element new_elem_mv;
	vertex node_mv;
	moved = copy_and_make_move_C4(game_state,0,move - 1, player);
		 if (moved == NULL){
			 //parent is stil "normal". remove him and get a normal tree
			 remove_tree(mainRoot,1,1);
			 return -1;
		 }
		// create new node
		node_mv = make_node(move, moved, get_state_score_C4(moved,0));
		if (node_mv == NULL){
			free(moved);
			//parent is stil "normal". remove him and get a normal tree
			remove_tree(mainRoot,1,1);
			return -1;
		}
		new_elem_mv = new_element();
		if (new_elem_mv == NULL){
			free(moved);
			free(node_mv->edges);
			free(node_mv);
			//parent is stil "normal". remove him and get a normal tree
			remove_tree(mainRoot,1,1);
			return -1;
		}
		new_elem_mv->node = node_mv;

		if (list->head == NULL)
		{	//if the list is empty the add new_elem_mv to list
			list->head=new_elem_mv;
			list->tail=new_elem_mv;
		}
		else
		{	//if list not empty,set new element as tail
			element prevTail=list->tail;
			prevTail->next = new_elem_mv;
			new_elem_mv->prev = prevTail;
			list->tail=new_elem_mv;
		}
		return 1;
}


char* get_name_C4()
{
	return Connect4_NAME;
}

int* get_difficulty_levels_C4(){
	return Connect4_diffficulties;
}

int is_game_over_C4(int* game_state)
{
	int score=get_state_score_C4(game_state,0);
	if (is_board_full_C4() || score==INT_MAX || score==INT_MIN){
		return 1;
	}
	return 0;
}

int is_victory_C4(int* game_state)
{
	int score=get_state_score_C4(game_state,0);
	if (score==INT_MAX || score==INT_MIN){
		return 1;
	}
	return 0;
}

int C4_handle_mouse_button_down (SDL_Event *event, int* game_state, int player)
{
	int x=0,y=0;
	int comp_move,succes;
	x=event->button.x;
	y=event->button.y;
	/* elem get's the elemnt to update (grid slot) */

	if(is_game_over_C4(game_state))
	{
		return 0;
	}
	succes=C4_make_move(game_state,0,x/100,player);
	if(succes==0)
	{
		return 0;
	}
	/*if (is_game_over_C4(game_state))
	{
		return 0;
	}*/
	return 1;
}

int	C4_handle_computer_turn(int* game_state, int depth, int player)
{
	int comp_move;
	if (player==-1){
		comp_move = get_computer_move(game_state, depth, get_state_children_C4);
	}
	else {
		comp_move=get_suggested_move(game_state,depth, get_state_children_C4);
	}
	C4_make_move(game_state,0,comp_move-1,player);
	return 0;
}

int	C4_make_move(int* game_state, int row, int col, int player)
{
	int i;
for (i = GROWS - 1; i >= 0; i--) // find appropriate row for column move
	{
		if (game_state[i*GCOLS + col] == 0)
		{
			game_state[i*GCOLS + col] = player;
			return 1;
		}
	}
	return 0;
}