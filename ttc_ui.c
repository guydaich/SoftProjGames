# include "ttc_ui.h"
# include "controls.h"

element_cntrl ttc_panel_function(int* game_state,void  (*makeMove)(void* cur_game,element_cntrl* ui_tree,int *quit,SDL_Event* test_event))
{ 
	control *ttc_grid;
	control *ttc_button;
	element_cntrl root, grid, temp;
	linked_list_cntrl list;
	int i,j;
 	
	root = new_control_element(new_panel(0,0,600,600,255,255,255));
	/*create panel children*/	
	list = new_control_list();
	/* grid surface - create control and element*/
	ttc_grid = new_button(0,0,TTC_GRIDPATH,0,"",1);	
	ttc_grid->pressedButton=makeMove;
	grid = new_control_element(ttc_grid);
	/* add grid to children list*/
	add_control_element_to_list(list,grid);
	/* update root children, and grid parent*/
	set_list_as_children(list,root);
	
	/*create grid children*/
	list = new_control_list();
	for (i=0; i< TIC_TAC_TOE_COLS; i++)
	{
		for(j=0; j< TIC_TAC_TOE_ROWS; j++)
		{
			ttc_button= NULL;
			if (game_state[i*TIC_TAC_TOE_ROWS + j] == TTC_PLAYER_1)	
			{
				ttc_button=new_button(j*TTC_WBTN+TTC_XOFFSET,i*TTC_HBTN+TTC_XOFFSET,TTC_BTNXPATH,1,NULL,0);
			}
			else if (game_state[i*TIC_TAC_TOE_ROWS + j] == TTC_PLAYER_2)	
			{
				ttc_button=new_button(j*TTC_WBTN+TTC_XOFFSET,i*TTC_HBTN+TTC_XOFFSET,TTC_BTNOPATH,1,NULL,0);
			}
			/*else
			{
				ttc_button=new_button(j*TTC_WBTN+TTC_XOFFSET,i*TTC_HBTN+TTC_XOFFSET,TTC_BTNEPATH,1,NULL,1);
			}*/
			/*add pieces to children list*/
			if (ttc_button != NULL)
			{
				temp = new_control_element(ttc_button);
				add_control_element_to_list(list,temp);
			}
		}
	}
	/*update parent-children*/
	set_list_as_children(list,grid);

	return root;
}