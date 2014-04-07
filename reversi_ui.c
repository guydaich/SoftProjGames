#include "reversi_ui.h"

element_cntrl	rv_panel_function(int* game_state,void  (*makeMove)(int *quit,SDL_Event* test_event))
{
	control *rv_grid;
	control *rv_button;
	element_cntrl root, grid, temp, children_panel;
	linked_list_cntrl list;
	int i,j;
 	
	/*create root panel and grid as a child*/
	root = new_control_element(new_panel(0,0,600,600,255,255,255,1));
	list = new_control_list();
	rv_grid = new_button(0,0,RVR_GRIDPATH,1,"",1);
	rv_grid->pressedButton=makeMove;
	grid = new_control_element(rv_grid);
	add_control_element_to_list(list,grid);
	set_list_as_children(list,root);
	
	/*panel to organise children within the game board*/
	children_panel = new_control_element(new_panel(49,49,598,598,255,255,255,0));
	list = new_control_list();
	add_control_element_to_list(list,children_panel);
	set_list_as_children(list,grid);

	/*create grid children - the board pieces*/
	list = new_control_list();
	for (i=0; i< REVERSI_ROWS; i++)
	{
		for(j=0; j< REVERSI_COLS; j++)
		{
			rv_button= NULL;
			if (game_state[i*REVERSI_ROWS + j] == REVERSI_PLAYER_1)	
			{
				rv_button=new_button((j)*RVR_WBTN,(i)*RVR_HBTN,RVR_BTN_BLACK_PATH,1,"",0);
			}
			else if (game_state[i*REVERSI_ROWS + j] == REVERSI_PLAYER_2)	
			{
				rv_button=new_button((j)*RVR_WBTN,(i)*RVR_HBTN,RVR_BTN_WHITE_PATH,1,"",0);
			}
			if (rv_button != NULL)
			{
				temp = new_control_element(rv_button);
				add_control_element_to_list(list,temp);
			}
		}
	}
	/*update parent-children*/
	set_list_as_children(list,children_panel);

	return root;
}
