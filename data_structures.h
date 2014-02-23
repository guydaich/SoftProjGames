#include <stdio.h>
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
	int				alpha;
	int				beta;
};

typedef struct  vertex_s * vertex;

#define EMPTY 0 // game Matrix Terms
#define HUMAN 1
#define COMPUTER -1


/*data structure functions*/
vertex new_node(int move, int *game_mtx_ptr, int score);
linked_list new_list();
element new_element();

