/* minimax.h */
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


/*definitions */
#define COLS 17 // Visual Board sizes
#define ROWS 8

#define GCOLS 7 // Actual Game Matrix Size
#define GROWS 6

#define EMPTY 0 // game Matrix Terms
#define HUMAN 1
#define COMPUTER -1


/* interface functions */
int get_suggested_move(int * game_matrix, int depth,int (*scoring_function)(int* game_matrix),int (*create_children)(vertex parent, int player));
int get_computer_move(int * gameMatrix,int depth,int (*scoring_function)(int* game_matrix),int (*create_children)(vertex parent, int player));
int getMove(vertex root);

/* evaluation functions */
//int scoring_function(int* game_matrix);

/*data structure functions*/
vertex make_node(int move, int *game_mtx_ptr, int score);
linked_list new_list();
element new_element();
//int create_children(vertex parent, int player, int isRoot);
void deleteList(element head,int is_nodes);
void remove_tree(vertex root, int is_root);
void remove_bad_subtree(vertex bad_subtree_root, int is_root);

/*minimax functions*/
//int * copy_board_move(board_t from, int move, int player);
vertex build_tree(int * game_matrix, int player, int maxDepth,int (*scoring_function)(int* game_matrix),int (*create_children)(vertex parent, int player));
int recursive_tree_build(vertex root, int player, int depth, int max_depth,int (*create_children)(vertex parent, int player));
//void fix_score(vertex root_node, int player);
int calc_min_max_value(int new_score, int player, int prev_score);
void killInfested(vertex infestedFather, int is_root);
int alphaBeta(vertex Node,int alpha, int beta,int player,int depth,int maxdepth);
