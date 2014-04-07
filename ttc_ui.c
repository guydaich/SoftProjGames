# include "ttc_ui.h"
# include "controls.h"


element_cntrl ttc_panel_function(int* game_state,void  (*makeMove)(void* cur_game,element_cntrl* ui_tree,int *quit,SDL_Event* test_event))
{ 
	control *ttc_grid;
	control *ttc_button;
	element_cntrl root, grid, temp, children_panel;
	linked_list_cntrl list;
	int i,j;
 	
	/* main panel */
	root = new_control_element(new_panel(0,0,600,600,255,255,255,1));
	/*grid picture as panel child*/
	list = new_control_list();
	ttc_grid = new_button(0,0,TTC_GRIDPATH,0,"",1);	
	ttc_grid->pressedButton=makeMove;
	grid = new_control_element(ttc_grid);
	add_control_element_to_list(list,grid);
	set_list_as_children(list,root);
	/*pieces panel, as grid child*/
	children_panel = new_control_element(new_panel(50,50,600,600,255,255,255,0));
	list = new_control_list();
	add_control_element_to_list(list,children_panel);
	set_list_as_children(list,grid);

	/*create grid children*/
	list = new_control_list();
	for (i=0; i< TIC_TAC_TOE_COLS; i++)
	{
		for(j=0; j< TIC_TAC_TOE_ROWS; j++)
		{
			ttc_button= NULL;
			if (game_state[i*TIC_TAC_TOE_ROWS + j] == TTC_PLAYER_1)	
			{
				ttc_button=new_button(j*TTC_WBTN,i*TTC_HBTN,TTC_BTNXPATH,1,NULL,0);
			}
			else if (game_state[i*TIC_TAC_TOE_ROWS + j] == TTC_PLAYER_2)	
			{
				ttc_button=new_button(j*TTC_WBTN,i*TTC_HBTN,TTC_BTNOPATH,1,NULL,0);
			}
			/*add pieces to children list*/
			if (ttc_button != NULL)
			{
				temp = new_control_element(ttc_button);
				add_control_element_to_list(list,temp);
			}
		}
	}
	/*update parent-children*/
	set_list_as_children(list,children_panel);

	return root;
}

/* detect the winning configuration of the game
 * update winning pieces to red background */
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
				set_victory_control(i,j,gamePanel,player);
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
				set_victory_control(j,i,gamePanel,player);
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
			set_victory_control(i,i,gamePanel,player);
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
			set_victory_control(i,TIC_TAC_TOE_ROWS-i-1,gamePanel,player);
		}
		return;
	}
}

/* function accepts TTC board matrix coordinates, 
 * locates the relevant TTC image, and creates 
 * a striked child underneath*/
void set_victory_control(int i,int j,element_cntrl game_panel,int player){
	element_cntrl ttc_button;
	int k;

	/* TTC pieces in ui tree */
	ttc_button=game_panel->children->head->children->head->children->head;
	/*advance in grid, until reaching piece i,j*/
	
	for(k=0; k < (TIC_TAC_TOE_ROWS-1)*TIC_TAC_TOE_COLS+TIC_TAC_TOE_COLS-1; k++)
	{	
		if(ttc_button->cntrl->x==j*TTC_WBTN 
			&& ttc_button->cntrl->y==i*TTC_HBTN) {
			break;
		}
		ttc_button=ttc_button->next;
	}

	/*Choose Striked X or O*/
	if (player==1){
		ttc_button->cntrl=new_button(ttc_button->cntrl->x,
			ttc_button->cntrl->y,TTC_BTNXPATH_VICTORY,1,NULL,0);
		// free previous control
		free_control(ttc_button->cntrl); 
	}
	else {
		ttc_button->cntrl=new_button(ttc_button->cntrl->x,
			ttc_button->cntrl->y,TTC_BTNOPATH_VICTORY,1,NULL,0);
		// free previous control
		free_control(ttc_button->cntrl); 
	}
}
