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

/*frees memory allocated for the node*/
void free_node(vertex node)
{
	if (node->game_state != NULL){
		free(node->game_state); 
	}
	free(node);
	return;
}

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