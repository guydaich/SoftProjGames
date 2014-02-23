#include "data_structures.h"
/* adds children to a node in the process of building the tree*/

/*creates and returns a new list*/
linked_list new_list()
{
	// allocate memory for new list
	linked_list new_list = (linked_list)malloc(sizeof(struct  linked_list_s));	

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
	new_node->alpha =		INT_MAX;
	new_node->beta =		INT_MIN;
	
	return new_node;
}

