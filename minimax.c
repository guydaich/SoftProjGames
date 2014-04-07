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
	vertex root = build_tree(game_matrix, 1,create_children);	// build tree
	if (root == NULL){
		exit(0);
	}
	alphaBeta(root,INT_MIN,INT_MAX,1,0,depth,create_children);								// update scores in the tree
	cMove = getMove(root);								// find best move
	remove_tree(root, 1,0);								// destroy tree
	return cMove;
}

/* choose the best next move for the computer, according to minimax */
int get_computer_move(int * game_matrix,int depth,linked_list (*create_children)(int *gameMatrix, int player))
{
	int cMove = 0;
	vertex root = build_tree(game_matrix, -1,create_children);
	if (root == NULL){
		exit(0);
	}
	alphaBeta(root,INT_MIN,INT_MAX,-1,0,depth,create_children);
	cMove = getMove(root);
	remove_tree(root, 1,0);
	return cMove;
}

/*return the move that this vertex represents*/
int getMove(vertex root)
{
	element cur_elem;
	int movement;
	for (cur_elem = root->edges->head; cur_elem != NULL;
		cur_elem = cur_elem->next)
	{
		if (cur_elem->node->score == root->score)
		{
			movement=cur_elem->node->mov_col;
			for (; cur_elem != NULL;cur_elem = cur_elem->next){
				remove_tree(cur_elem->node, 0,0);
		}
			return movement;
		}
		remove_tree(cur_elem->node, 0,0);
	}
	return 0;
}
/* ---- END Interface function ---- */

vertex build_tree(int * game_matrix, int player,linked_list (*create_children)(int *gameMatrix, int player))
{
	//int error;
	linked_list new_children;
	vertex root = make_node(0, game_matrix,0); 			// new root 
	if (root == NULL){
		return NULL;
	}
	new_children=create_children(root->game_state, player);	// children belong to opposite player
	root->edges=new_children;
	return root;
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
	new_node->edges = NULL;
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

void remove_tree(vertex root, int is_root,int recursivly)
{
	if (root->edges!=NULL){
	if (is_root==1){
			deleteList(root->edges->head,1,recursivly);
	}
	else{
			deleteList(root->edges->head,1,recursivly);
	}
	free(root->edges);
	numOfElemmalList--;
	}
	if (is_root==0){
		free(root->game_state);
		boardCount--;
	}
	free(root);
	numOfNodemal--;
}

void deleteList(element head,int is_nodes,int recursivly){
	if(head==NULL){
		return;
	}
	deleteList(head->next,is_nodes,recursivly);
	if (recursivly==1){
		remove_tree(head->node,0,recursivly);
	}
	free(head);
	numE--;
}


int alphaBeta(vertex Node,int alpha, int beta,int player,int depth,int maxdepth,linked_list (*create_children)(int *gameMatrix, int player)){
	int aboveAlpha=alpha,aboveBeta=beta;
	int temp;
	vertex child;
	element run;
	linked_list new_children;
	if (depth>=maxdepth || Node->edges->head==NULL){
		temp= Node->score;
		if(depth>1){
			remove_tree(Node,0,0);
		}
		return temp;
	}
	if (player==HUMAN){
		for (run = Node->edges->head;run != NULL; run = run->next){
			child=run->node;					// choose node	
			if (depth<maxdepth-1){
				new_children=create_children(child->game_state, player*-1);			// add children
				child->edges=new_children;
			}
			temp=alphaBeta(child,alpha,beta,player*-1,depth+1,maxdepth,create_children);
			if(aboveAlpha < temp){
				aboveAlpha=temp;
			}
			if (aboveBeta<=aboveAlpha){
				for ( run = run->next;run != NULL; run = run->next){
					child=run->node;
					if(depth>1){
						remove_tree(child,0,0);
					}
				}
				break;
			}
			
		}
		Node->score=aboveAlpha;
		if(depth>1){
			remove_tree(Node,0,0);
		}
		return aboveAlpha;
	}
	else {
		for (run = Node->edges->head;run != NULL; run = run->next){
			child=run->node;
			if (depth<maxdepth-1){
				new_children=create_children(child->game_state, player*-1);			// add children
				child->edges=new_children;
			}
			temp=alphaBeta(child,alpha,beta,player*-1,depth+1,maxdepth,create_children);
			if(aboveBeta > temp){
				aboveBeta=temp;
			}
			if (aboveBeta<=aboveAlpha){
				for ( run = run->next;run != NULL; run = run->next){
					child=run->node;
					if(depth>1){
						remove_tree(child,0,0);
					}
				}
				break;
			}
		}
		Node->score=aboveBeta;
		if(depth>1){
			remove_tree(Node,0,0);
		}
		return aboveBeta;
	}
}