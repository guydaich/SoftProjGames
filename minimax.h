/* minimax.h */
#ifndef MINIMAX_H
#define MINIMAX_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <limits.h>
#include <stdlib.h>

/*we use the data structure in the minimax module*/
struct element_s {
	struct vertex_s		*node;
	struct element_s	*next;
	struct element_s	*prev;
};

typedef struct element_s *  element;
typedef int* board_t;//CHANGED from int**

struct linked_list_s {
	element head;
	element tail;
};

typedef struct linked_list_s *  linked_list;
struct vertex_s {
	unsigned int	mov_col;//0 for root
	board_t			game_state;
	int				score;
	linked_list		edges;
};

typedef struct  vertex_s * vertex;

#define GCOLS 7 // Actual Game Matrix Size
#define GROWS 6

enum piece{
	EMPTY=0,HUMAN=1,COMPUTER=-1
};
typedef enum piece piece;


/* interface functions */
int get_suggested_move(int * game_matrix, int depth,linked_list (*create_children)(int *gameMatrix, int player,int *error));
int get_computer_move(int * gameMatrix,int depth,linked_list (*create_children)(int *gameMatrix, int player,int *error));
int getMove(vertex root);

/*data structure functions*/
vertex make_node(int move, int *game_mtx_ptr, int score);
linked_list new_list();
element new_element();
void deleteList(element head,int is_nodes,int recursivly);
void remove_tree(vertex root, int is_root,int recursivly);
//void free_node(vertex node);

/*minimax functions*/
vertex build_tree(int * game_matrix, int player,linked_list (*create_children)(int *gameMatrix, int player,int *error));
int calc_min_max_value(int new_score, int player, int prev_score);
int alphaBeta(vertex Node,int alpha, int beta,int player,int depth,int maxdepth,linked_list (*create_children)(int *gameMatrix, int player,int *error),int* error);

#endif
