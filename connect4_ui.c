#include "connect4_ui.h"

element_cntrl	C4_panel_function(int* game_state,void  (*makeMove)(void* cur_game,element_cntrl* ui_tree,int *quit,SDL_Event* test_event))
{
	control *C4_grid;
	control *C4_button;
	element_cntrl root, grid, temp, children_panel;
	linked_list_cntrl list;
	int i,j;
 	
	root = new_control_element(new_panel(0,0,C4_WGRID,C4_HGRID,255,255,255,1));
	/*create panel children*/	
	list = new_control_list();
	/* grid surface - create control and element*/
	C4_grid = new_button(0,0,C4_GRIDPATH,0,NULL,1);
	C4_grid->pressedButton=makeMove;
	grid = new_control_element(C4_grid);
	/* add grid to children list*/
	add_control_element_to_list(list,grid);
	/* update root children, and grid parent*/
	set_list_as_children(list,root);
	
	children_panel = new_control_element(new_panel(C4_XOFFSET,C4_YOFFSET,600,600,255,255,255,0));
	list = new_control_list();
	add_control_element_to_list(list,children_panel);
	set_list_as_children(list,grid);

	/*create grid children*/
	list = new_control_list();
	for (i=0; i< CONNECT4_ROWS; i++)
	{
		for(j=0; j< CONNECT4_COLS; j++)
		{
			C4_button= NULL;
			if (game_state[i*CONNECT4_COLS + j] == CONNECT4_PLAYER_1)	
			{
				C4_button=new_button((j)*C4_WBTN,(i)*C4_HBTN,C4_BTN_RED_PATH,1,NULL,0);
			}
			else if (game_state[i*CONNECT4_COLS + j] == CONNECT4_PLAYER_2)	
			{
				C4_button=new_button((j)*C4_WBTN,(i)*C4_HBTN,C4_BTN_BLUE_PATH,1,NULL,0);
			}
			/*add pieces to children list*/
			if (C4_button != NULL)
			{
				temp = new_control_element(C4_button);
				add_control_element_to_list(list,temp);
			}
		}
	}
	/*update parent-children*/
	set_list_as_children(list,children_panel);

	return root;
}
