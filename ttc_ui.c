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
				ttc_button=new_button(j*TTC_WBTN+TTC_XOFFSET,i*TTC_HBTN+TTC_YOFFSET,TTC_BTNXPATH,1,NULL,0);
			}
			else if (game_state[i*TIC_TAC_TOE_ROWS + j] == TTC_PLAYER_2)	
			{
				ttc_button=new_button(j*TTC_WBTN+TTC_XOFFSET,i*TTC_HBTN+TTC_YOFFSET,TTC_BTNOPATH,1,NULL,0);
			}
			/*else
			{
				ttc_button=new_button(j*TTC_WBTN+TTC_XOFFSET,i*TTC_HBTN+TTC_YOFFSET,TTC_BTNEPATH,1,NULL,1);
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

void color_ttc(int* game_state,int player,element_cntrl ui_tree){
	element_cntrl gamePanel;
	int i,j,flag; 
	/* check lines */	
	
	gamePanel=ui_tree->children->tail;
		// check rows
	for (i=0; i< TIC_TAC_TOE_ROWS; i++)
	{
		flag = 1;
		for(j=0; j< TIC_TAC_TOE_COLS; j++)
		{
			if (game_state[i*TIC_TAC_TOE_ROWS + j] != player)
			{
				flag = 0;
			}
		}
		if (flag)
		{
			for(j=0; j< TIC_TAC_TOE_COLS; j++){
				setVic(i,j,gamePanel,player);
			}
			return;
		}
	}

		// check cols
	for (i=0; i< TIC_TAC_TOE_COLS; i++)
	{
		flag = 1;
		for(j=0; j< TIC_TAC_TOE_ROWS; j++)
	{
		if (game_state[j*TIC_TAC_TOE_ROWS + i] != player)
			{
				flag = 0;
			}
		}
		if (flag)
		{
			for(j=0; j< TIC_TAC_TOE_ROWS; j++){
				setVic(j,i,gamePanel,player);
			}
			return;
		}
	}

	// diagonal 1
	flag = 1;
	for (i=0; i< TIC_TAC_TOE_ROWS; i++)
	{
		if (game_state[i*TIC_TAC_TOE_ROWS + i] != player)
		{
			flag = 0;
		}	
	}
	if (flag)
	{
		for (i=0; i< TIC_TAC_TOE_ROWS; i++){
			setVic(i,i,gamePanel,player);
		}
		return;
	}

	// diagonal 2
	flag = 1;
	for (i= TIC_TAC_TOE_ROWS -1; i >=0 ; i--)
		{
			if (game_state[i*TIC_TAC_TOE_ROWS + (TIC_TAC_TOE_ROWS-i-1)] != player)
			{
				flag = 0;
			}
		}

	if (flag)
	{
		for (i= TIC_TAC_TOE_ROWS -1; i >=0 ; i--){
			setVic(i,TIC_TAC_TOE_ROWS-i-1,gamePanel,player);
		}
		return;
	}
}

void setVic(int i,int j,element_cntrl game_panel,int player){
	element_cntrl ttc_button,temp;
	control *ttc_button_victory;
	linked_list_cntrl list;
	int k;

	ttc_button=game_panel->children->head->children->head;
	for(k=0;k<(TIC_TAC_TOE_ROWS-1)*TIC_TAC_TOE_COLS+TIC_TAC_TOE_COLS-1;k++){
		if(ttc_button->cntrl->x==j*TTC_WBTN+TTC_XOFFSET && ttc_button->cntrl->y==i*TTC_HBTN+TTC_YOFFSET) {
			break;
		}
		ttc_button=ttc_button->next;
	}

	if (player==1){
		ttc_button_victory=new_button(0,0,TTC_BTNXPATH_VICTORY,1,NULL,0);
	}
	else {
		ttc_button_victory=new_button(0,0,TTC_BTNOPATH_VICTORY,1,NULL,0);
	}
	list = new_control_list();

	temp = new_control_element(ttc_button_victory);
	add_control_element_to_list(list,temp);
	set_list_as_children(list,ttc_button);
}
