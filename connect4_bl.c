/* connect4_bl.c */
/* game logic class for the connect-4 (henceforth, C4) game
 * Supplies all game functions required by game implmntation: 
 * init, name, difficulties, state_children. the panel_function
 * can be found in connect4_ui.c */
 
#include "connect4_bl.h"

/* game difficulties */
int Connect4_diffficulties[] = {1,2,3,4,5,6,7};

/* init a C4 game board, with no pieces.
 * retunn board pointer, on failure return null*/
int *get_initial_state_C4()
{
	int i,j = 0;
	int *game_matrix=(int *)calloc(CONNECT4_ROWS*CONNECT4_COLS,sizeof(int));
	if (game_matrix==NULL){
		return NULL;
	}
	for (i =0; i < CONNECT4_ROWS ; i++)
	{
		for (j=0; j< CONNECT4_ROWS; j++)
		{
			(game_matrix)[i*CONNECT4_COLS+j] = 0; 
		}
	}
	return game_matrix;
}

/* determines if a given C4 boars is full.
 * returns 1 if board is full, 0 otherwise */
int is_board_full_C4(int* game_state)
{
	int i =0,j=0;

	for (j = 0; i < CONNECT4_ROWS; ++i)
	{
		for (i=0; i<CONNECT4_COLS; i++)
		{
			if(game_state[j*CONNECT4_COLS+i]==0){
				return 0;
			}
		}
	}
	return 1;
}

/* scoring function for C4, as detailed in peoject
* in case of player 1 victory returns INT_MAX,
* and in case of player 2 victory returns INT_MIN*/
int get_state_score_C4(int* game_matrix)
{
	int sizesOFLine[8] = { 0 };
	int weightVector[6] = { -5, -2, -1, 1, 2, 5 };
	int i, j, k, lineScore = 0, totalScore = 0;

	//lines in rows calculation
	for (i = 0; i < CONNECT4_ROWS; i++) {
		for (j = 0; j < CONNECT4_COLS - 3; j++) {
			lineScore = 0;
			for (k = 0; k < 4; k++) {
				if (game_matrix[(i)*CONNECT4_COLS + j + k] == ENUM_PLAYER_1){
					lineScore++;
				}
				else if (game_matrix[(i)*CONNECT4_COLS + j + k] == ENUM_PLAYER_2){
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
	for (j = 0; j < CONNECT4_COLS; j++) {
		for (i = 0; i < CONNECT4_ROWS - 3; i++) {
			lineScore = 0;
			for (k = 0; k < 4; k++) {
				if (game_matrix[(i + k)*CONNECT4_COLS + j] == ENUM_PLAYER_1){
					lineScore++;
				}
				else if (game_matrix[(i + k)*CONNECT4_COLS + j] == ENUM_PLAYER_2){
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
	for (j = 0; j < CONNECT4_COLS; j++) {
		for (i = 0; i < CONNECT4_ROWS; i++) {
			lineScore = 0;
			if (((i + 3) < CONNECT4_ROWS) && ((j + 3) < CONNECT4_COLS)){
				for (k = 0; k < 4; k++) {
					if (game_matrix[(i + k)*CONNECT4_COLS + j + k] == ENUM_PLAYER_1){
						lineScore++;
					}
					else if (game_matrix[(i + k)*CONNECT4_COLS + j + k] == ENUM_PLAYER_2){
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
			if (((i - 3) > -1) && ((j + 3) < CONNECT4_COLS)){//diganals which go down and right
				for (k = 0; k < 4; k++) {
					if (game_matrix[(i - k)*CONNECT4_COLS + j + k] == ENUM_PLAYER_1){
						lineScore++;
					}
					else if (game_matrix[(i - k)*CONNECT4_COLS + j + k] == ENUM_PLAYER_2){
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
		return INT_MIN; //Player 2 won
	}
	if (sizesOFLine[7] > 0){
		return INT_MAX; //Player 1 won
	}

	for (i = 1; i < 7; i++){
		totalScore = totalScore + sizesOFLine[i] * weightVector[i - 1];
	}
	return totalScore;
}

/* allocates a copy of from, and and according to move_col, makes a move
 * returns null on failure, and a pointer to the new board otherwise */
int * copy_and_make_move_C4(board_t from, int move_col, int player) 
{
	int i, j;
	int *new_board_ptr = // allocate new board
		(int*)calloc(CONNECT4_ROWS*CONNECT4_COLS, sizeof(int));	
	if (new_board_ptr == NULL)
	{
		perror("Error: standard function calloc has failed");
		return NULL;
	}

	boardCount++;

	for (i = 0; i < CONNECT4_ROWS; i++)		// copy board
	{
		for (j = 0; j < CONNECT4_COLS; j++)
		{
			new_board_ptr[i*CONNECT4_COLS + j] = (from)[i*CONNECT4_COLS + j];
		}
	}
	
	for (i = CONNECT4_ROWS - 1; i >= 0; i--) // find appropriate row for column move and make move
	{
		if (new_board_ptr[i*CONNECT4_COLS + move_col] == 0)
		{
			new_board_ptr[i*CONNECT4_COLS + move_col] = player;
			break;
		}
	}
	return new_board_ptr;
}

/* This functions returns a list of game state element, for minimax use.
 * Each of these elements represents a possible move that a player can do 
 * in his turn, where game_state is the current game_state.
 * on success the function returns with *error=0, on failure returns with *error=-1*/
linked_list get_state_children_C4(int* matrix, int player,int *error)
{
	linked_list newList;
	int move;
	int addChildError=0;
	int child_player = player;
	element run_elem;

	newList=new_list();
	if (newList==NULL){
		*error=-1;
		return NULL;
	}

	if (get_state_score_C4(matrix) == INT_MAX ||
		get_state_score_C4(matrix) == INT_MIN)		//if victory achieved stop building
	{
		return newList;
	}

	for (move = 1; move < 8; move++)
	{
		if ((matrix)[move - 1] != 0)	// if game doesn't allow this move
		{
			continue;								// continue to next move
		}
		addChildError=add_to_children_list_C4( newList,matrix,move,child_player);
		if(addChildError==-1){
			break;
		}
	}
	if(addChildError==-1){
		for (run_elem=newList->head;run_elem!=NULL && run_elem->next!=NULL;run_elem=run_elem->next){
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
		free(newList);
		*error=-1;
		return NULL;
	}
	return newList;
}

/* creates a minimax node and element for each child-state 
 * (game_state and move), and adds to list. 
 * 0 on success, on failure returns -1*/
int add_to_children_list_C4(linked_list list, int* game_state, int col, int player)
{
	int move=col;
	int* moved;
	element new_elem_mv;
	vertex node_mv;
	moved = copy_and_make_move_C4(game_state,move - 1, player);
	if (moved == NULL){
		return -1;
	}
	// create new node
	node_mv = make_node(move, moved, get_state_score_C4(moved));
	if (node_mv == NULL){
		free(moved);
		return -1;
	}
	new_elem_mv = new_element();
	if (new_elem_mv == NULL){
		free(moved);
		free(node_mv);
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

/* returns game name */
char* get_name_C4()
{
	return Connect4_NAME;
}

/* get difficult levels for game*/
int* get_difficulty_levels_C4(){
	return Connect4_diffficulties;
}

/*for given board, if the game is over (full board or victory) return 1. else return 0*/
int is_game_over_C4(int* game_state)
{
	int score=get_state_score_C4(game_state);
	if (is_board_full_C4(game_state) || score==INT_MAX || score==INT_MIN){
		return 1;
	}
	return 0;
}

/*for given board, if one of the players achived victory return 1. else return 0*/
int is_victory_C4(int* game_state)
{
	int score=get_state_score_C4(game_state);
	if (score==INT_MAX ){
		return 1;
	}
	if (score==INT_MIN){
		return -1;
	}
	return 0;
}

/* interpert game mouse button down, to game matrix position
 * attempt to make the requested move, and return 1 on success, 0 OW*/
int C4_handle_mouse_button_down (SDL_Event *event, int* game_state, int player)
{
	int x=0;
	int succes;
	x=event->button.x;
	/* elem get's the elemnt to update (grid slot) */

	if(is_game_over_C4(game_state))
	{
		return 0;
	}
	succes=C4_make_move(game_state,0,(x-C4_XOFFSET)/C4_WBTN,player);
	if(succes==0)
	{
		return 0;
	}
	return 1;
}

/* Calls minimax to get AIs next move */
int	C4_handle_computer_turn(int* game_state, int depth, int player)
{
	int comp_move;
	if (player==-1){
		comp_move = get_computer_move(game_state, depth, get_state_children_C4);
	}
	else {
		comp_move=get_suggested_move(game_state,depth, get_state_children_C4);
	}
	if(comp_move<0){
		return -1;
	}
	else if (comp_move==0){//zero cann't be valid move in this game
		return -2;
	}
	C4_make_move(game_state,0,comp_move-1,player);//return 0 doesn't mean error??
	return 0;
}

/* given a state and col, determines row of insertion for new piece */
int	C4_make_move(int* game_state, int row, int col, int player)
{
	int i;
	for (i = CONNECT4_ROWS - 1; i >= 0; i--) // find appropriate row for column move
	{
		if (game_state[i*CONNECT4_COLS + col] == 0)
		{
			game_state[i*CONNECT4_COLS + col] = player;
			return 1;
		}
	}
	return 0;
}