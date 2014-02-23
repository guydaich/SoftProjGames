/*						minimax.c								*/
/* module handles game minimax algorithm: tree construction,	*/
/* mainatanance, score calculations, move suggestion			*/


#include "minimax.h"

vertex mainRoot;
int boardCount;
int numOfElemmalList;
int numOfNodemal;
int numE;



/* ---- Interface functions ---- */

/* returns a suggested move foe the player, according to minimax */
int get_suggested_move(int * game_matrix, int depth,linked_list (*create_children)(int *gameMatrix, int player))
{
	int cMove = 0; 
	vertex root = build_tree(game_matrix, 1, depth,create_children);	// build tree
	if (root == NULL){
		exit(0);
	}
	alphaBeta(root,INT_MIN,INT_MAX,1,0,depth);								// update scores in the tree
	cMove = getMove(root);								// find best move
	remove_tree(root, 1);								// destroy tree
	return cMove;
}

/* choose the best next move for the computer, according to minimax */
int get_computer_move(int * game_matrix,int depth,linked_list (*create_children)(int *gameMatrix, int player))
{
	int cMove = 0;
	vertex root = build_tree(game_matrix, -1, depth,create_children);
	if (root == NULL){
		exit(0);
	}
	alphaBeta(root,INT_MIN,INT_MAX,-1,0,depth);
	cMove = getMove(root);
	remove_tree(root, 1);
	return cMove;
}

/*return the move that this vertex represents*/
int getMove(vertex root)
{
	element cur_elem;
	for (cur_elem = root->edges->head; cur_elem != NULL;
		cur_elem = cur_elem->next)
	{
		if (cur_elem->node->score == root->score)
		{
			return cur_elem->node->mov_col;
		}
	}
	return 0;
}
/* ---- END Interface function ---- */

/* Data Structure Functions */
/* assign a root and calls recursive tree construction */
vertex build_tree(int * game_matrix, int player, int maxDepth,linked_list (*create_children)(int *gameMatrix, int player))
{
	//int error;
	linked_list new_children;
	vertex root = make_node(0, game_matrix,0); 			// new root 
	if (root == NULL){
		return NULL;
	}
	new_children=create_children(root->game_state, player);	// children belong to opposite player
	root->edges=new_children;
	if (0 == -1){								// problem with std function
		remove_bad_subtree(root, 1);						// remove bad 
		return NULL;
	}
	recursive_tree_build(root, -1 * player, 1, maxDepth,create_children);	// construct tree	
	if (0 == -1){
		remove_tree(root, 1);
		return NULL;
	}
	return root;
}

/* constructs a tree, recursively, after root has been assigned */
int recursive_tree_build(vertex root, int player, int depth, int max_depth,linked_list (*create_children)(int *gameMatrix, int player))
{
	element cur_elem;
	vertex cur_node;
	linked_list new_children;
	if (depth >= max_depth)		// if exceeded maximal depth, stop tree construction 
	{
		return 0;
	}
	else
	{
		for (cur_elem = root->edges->head;					// iterate over elements
			cur_elem != NULL; cur_elem = cur_elem->next)
		{
			cur_node = cur_elem->node;						// choose node			
			new_children=create_children(cur_node->game_state, player);			// add children
			cur_node->edges=new_children;
			if (0 == -1){
				return 0;
			}
			recursive_tree_build(cur_node, -1 * player, depth + 1, max_depth,create_children);
			if (0 == -1){
				return 0;
			}
		}
	}
	return 0;
}

/* creates a new node, and returns it to Caller */
vertex make_node(int move, int *game_mtx_ptr, int score)
{
	vertex new_node = (vertex)malloc(sizeof(struct  vertex_s));
	numOfNodemal++;
	if (new_node==NULL){
		perror("Error: standard function malloc has failed");
		return NULL;
	}
	new_node->mov_col = move;
	new_node->game_state = game_mtx_ptr;
	new_node->score = score;
	new_node->edges = new_list();
	if (new_node->edges==NULL){
		free(game_mtx_ptr);
		free(new_node);
		return NULL;
	}
	return new_node;
}

/*creates and returns a new list*/
linked_list new_list()
{
	// allocate memory for new list
	linked_list new_list = 
		(linked_list)malloc(sizeof(struct  linked_list_s));	
	numOfElemmalList++;
	if (new_list == NULL)	// handle standart function error
	{
		perror("Error: standard function malloc has failed");
		return NULL;
	}
	new_list->head = NULL;	// set members to NULL
	new_list->tail = NULL;
	return new_list;
}

/*creates and returns a new elements*/
element new_element()
{
	// allocate memory for new element
	element new_elem = (element)malloc(sizeof(struct  element_s));
	numE++;
	if (new_elem == NULL)	// handle standart function error
	{
		perror("Error: standard function malloc has failed");
		return NULL;
	}
	new_elem->node = NULL;		// set members to NULL
	new_elem->next = NULL;
	new_elem->prev = NULL;
	return new_elem;
}

vertex new_node(int move, int *game_state, int score)
{
	vertex new_node = (vertex)malloc(sizeof(struct  vertex_s));

	if (new_node==NULL)
	{
		perror("Error: standard function malloc has failed");
		return NULL;
	}
	
	new_node->mov_col =		move;
	new_node->game_state =	game_state;
	new_node->score =		score;
	new_node->edges =		new_list();
	
	return new_node;
}


/* adds children to a node in the process of building the tree
int create_children(vertex parent, int player,int isRoot)
{
	element new_elem_mv;
	vertex node_mv;
	int* moved;
	int move;
	int child_player = -1 * player;		// player is opposite

	if (parent->score == INT_MAX ||
		parent->score == INT_MIN)		//if victory achieved stop building
	{
		return 1;							//return
	}

	for (move = 1; move < 8; move++)
	{
		if (((parent->game_state))[move - 1] != 0)	// if game doesn't allow this move
		{
			continue;								// continue to next move
		}

		// make new identical board
		 moved = 
		 	copy_board_move(parent->game_state, move - 1, child_player);
		 if (moved == NULL){
			 remove_bad_subtree(parent, isRoot);
			 return -1;
		 }
		// create new node
		node_mv = make_node(move, moved, scoring_function(moved));
		if (node_mv == NULL){
			remove_bad_subtree(parent, isRoot);
			return -1;
		}
		new_elem_mv = new_element();
		if (new_elem_mv == NULL){
			remove_bad_subtree(parent, isRoot);
			return -1;
		}
		new_elem_mv->node = node_mv;

		if (parent->edges->head == NULL)
		{	//if the list is empty the add new_elem_mv to list
			parent->edges->head = new_elem_mv;
			parent->edges->tail = new_elem_mv;
		}
		else
		{	//if list not empty,set new element as tail
			element prevTail = parent->edges->tail;
			prevTail->next = new_elem_mv;
			new_elem_mv->prev = prevTail;
			parent->edges->tail = new_elem_mv;
		}
	}
	return 1;
}*/

/*removes tree by freeing all the memory previously allocated
void remove_tree(vertex root, int is_root)
{
	element cur_elem = NULL, prev_elem=NULL;
	for (cur_elem = root->edges->head; cur_elem != NULL && 
	cur_elem->next != NULL ; cur_elem = cur_elem->next)	//iterate over edges
	{
		remove_tree(cur_elem->node, 0);		// recursively call deletion on each child
		prev_elem = cur_elem->prev;			// choose previous sibling
		if (prev_elem != NULL)
		{
			free(prev_elem);				// free him, after his children were freed
		}
	}
	if (cur_elem != NULL && prev_elem!=NULL)
	{
		remove_tree(cur_elem->node, 0);		// GUY: what's going one here?
		free(cur_elem->prev);
		free(cur_elem);
	}
	else{
		if (cur_elem != NULL){
			remove_tree(cur_elem->node, 0);
			free(cur_elem);
		}
	}
	free(root->edges);
	if (is_root == 0)	//we won't free the root game state, to not hurt the board
		//GUY: but the board itself wasn't allocated here....
	{
		free(root->game_state);
	}
	free(root);
}*/

void remove_tree(vertex root, int is_root)
{
	if (is_root==1){
		deleteList(root->edges->head,1);
	}
	else{
		deleteList(root->edges->head,1);
	}
	free(root->edges);
	numOfElemmalList--;
	if (is_root==0){
		free(root->game_state);
		boardCount--;
	}
	free(root);
	numOfNodemal--;
}

void deleteList(element head,int is_nodes){
	if(head==NULL){
		return;
	}
	deleteList(head->next,is_nodes);
	remove_tree(head->node,0);
	free(head);
	numE--;
}


/*make a copy of the board
int * copy_board_move(board_t from, int move, int player) 
{
	int i, j;
	int *new_board_ptr = // allocate new board
		(int*)calloc(GROWS*GCOLS, sizeof(int));	
	
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
		if (new_board_ptr[i*GCOLS + move] == 0)
		{
			new_board_ptr[i*GCOLS + move] = player;
			break;
		}
	}
	return new_board_ptr;
}*/


/* Calculates Scoring for a Board*/
int scoring_function(int* game_matrix)	//asume game_matrix=2D array[GROWS][GCOLS]
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

/* calculates recursivle the score bottom up, from leaves*/
void fix_score(vertex root_node, int player)
{
	int min_max_value;
	int list_not_empty = 0;
	element cur_elem;
	vertex cur_node;

	for (cur_elem = root_node->edges->head;
		cur_elem != NULL; cur_elem = cur_elem->next) // itertate over edges
	{
		cur_node = cur_elem->node;
		fix_score(cur_node, -1 * player);

		if (list_not_empty == 0) 			// minmax of one the elem itself
		{
			min_max_value = cur_node->score;
			list_not_empty = 1;
		}
		else
		{
			min_max_value = calc_min_max_value(
					cur_node->score, player, min_max_value);
		}
	}

	if (list_not_empty == 1)	// final assignment
	{
		root_node->score = min_max_value;
	}
}



/* decides  where to minimize or maximize, according to player */
int calc_min_max_value(int new_score, int player, int prev_score)
{
	if (player == COMPUTER)
	{
		if (prev_score > new_score)
		{
			return new_score;
		}
		else
		{
			return prev_score;
		}
	}
	else
	{
		if (prev_score < new_score)
		{
			return new_score;
		}
		else
		{
			return prev_score;
		}
	}
}

/* ---- END Evaluation functions ---- */

/* in case of a bad allocation, remove the children of a that vertex*/
void remove_bad_subtree(vertex bad_subtree_root, int is_root)
{
	element cur_elem = NULL, prev_elem = NULL;
	// case 1: there are more than 2 children
	for (cur_elem = bad_subtree_root->edges->head;  //iterate over edges
		cur_elem != NULL && cur_elem->next != NULL ; cur_elem = cur_elem->next)			
	{
		prev_elem = cur_elem->prev;			// choose previous sibling
		if (prev_elem != NULL)
		{
			if (prev_elem->node != NULL){
				if (prev_elem->node->game_state != NULL){
					free(prev_elem->node->game_state);
				}
				if (prev_elem->node->edges != NULL){
					free(prev_elem->node->edges);
				}
				free(prev_elem->node);
			}
			free(prev_elem);				// free him, after his children were freed
		}
	}
	// case 2: there are 2 children
	if (cur_elem != NULL && prev_elem != NULL)
	{
		free(cur_elem->prev);
		if (cur_elem->node != NULL){
			if (cur_elem->node->game_state != NULL){
				free(cur_elem->node->game_state);
			}
			if (cur_elem->node->edges != NULL){
				free(cur_elem->node->edges);
			}
			free(cur_elem->node);
		}
		free(cur_elem);
	}
	// case 1: one child
	else{
		if (cur_elem->node != NULL){
			if (cur_elem->node->game_state != NULL){
				free(cur_elem->node->game_state);
			}
			if (cur_elem->node->edges != NULL){
				free(cur_elem->node->edges);
			}
			free(cur_elem->node);
		}
		if (cur_elem != NULL){
			free(cur_elem);
		}
	}
}

int alphaBeta(vertex Node,int alpha, int beta,int player,int depth,int maxdepth){
	int aboveAlpha=alpha,aboveBeta=beta;
	int temp;
	vertex child;
	element run;
	if (Node->edges->head==NULL){
		return Node->score;
	}
	if (player==HUMAN){
		for (run = Node->edges->head;run != NULL; run = run->next){
			child=run->node;
			temp=alphaBeta(child,alpha,beta,player*-1,depth+1,maxdepth);
			if(aboveAlpha < temp){
				aboveAlpha=temp;
			}
			if (aboveBeta<=aboveAlpha){
				break;
			}
			
		}
		Node->score=aboveAlpha;
		return aboveAlpha;
	}
	else {
		for (run = Node->edges->head;run != NULL; run = run->next){
			child=run->node;
			temp=alphaBeta(child,alpha,beta,player*-1,depth+1,maxdepth);
			if(aboveBeta > temp){
				aboveBeta=temp;
			}
			if (aboveBeta<=aboveAlpha){
				break;
			}
		}
		Node->score=aboveBeta;
		return aboveBeta;
	}
}

