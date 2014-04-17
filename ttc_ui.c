# include "ttc_ui.h"

extern int buttomNum;

element_cntrl ttc_panel_function(int* game_state,int  (*makeMove)(int *quit,SDL_Event* test_event))
{ 
	control *ttc_grid=NULL;
	control *ttc_button=NULL;
	element_cntrl root=NULL, grid=NULL, temp=NULL, children_panel=NULL;
	linked_list_cntrl list=NULL;
	int i=0,j=0,error =0;
 	
	/* main panel */
	root = new_control_element(new_panel(0,0,600,600,255,255,255,1));
	if (root==NULL){
		printf("can't make root in ttc_panel_function\n");
		return NULL;
	}
	/*grid picture as panel child*/
	list = new_control_list();
	if (list==NULL){
		printf("can't make list for root in ttc_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	ttc_grid = new_button(0,0,TTC_GRIDPATH,0,NULL,1);	
	if (ttc_grid==NULL){
		printf("can't make ttc_grid in ttc_panel_function\n");
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	ttc_grid->pressedButton=makeMove;
	grid = new_control_element(ttc_grid);
	if (grid==NULL){
		printf("can't make control element for ttc_grid in ttc_panel_function\n");
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	add_control_element_to_list(list,grid);
	set_list_as_children(list,root);
	/*pieces panel, as grid child*/
	children_panel = new_control_element(new_panel(50,50,600,600,255,255,255,0));
	if (children_panel==NULL){
		printf("can't make children_panel in ttc_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	list = new_control_list();
	if (list==NULL){
		printf("can't make list for ttc_grid in ttc_panel_function\n");
		free_control(children_panel->cntrl);
		free(children_panel);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	add_control_element_to_list(list,children_panel);
	set_list_as_children(list,grid);

	/*create grid children*/
	list = new_control_list();
	if (list==NULL){
		printf("can't make list for children_panel in ttc_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
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
				if (temp==NULL){
					printf("can't make control element for piece in ttc_panel_function\n");
					error=1;
					break;
				}
				add_control_element_to_list(list,temp);
			}
			else if(game_state[i*TIC_TAC_TOE_ROWS + j]==TTC_PLAYER_1 || game_state[i*TIC_TAC_TOE_ROWS + j] == TTC_PLAYER_2){
				printf("can't make piece in ttc_panel_function\n");
				error=1;
				break;
			}
		}
		if(error==1){
			break;
		}
	}
	if(error==1){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	//update parent-children
	set_list_as_children(list,children_panel);

	return root;
}

/* detect the winning configuration of the game
 * update winning pieces to red background */
int color_ttc(int* game_state,int player,element_cntrl ui_tree){
	element_cntrl gamePanel=NULL;
	int i=0,j=0,flag=1;
	int error=0; 	
	
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
				error=set_victory_control(i,j,gamePanel,player);
				if (error==-1){
					printf("failed to draw victory\n");
					return -1;
				}
			}
			return 0;
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
				error=set_victory_control(j,i,gamePanel,player);
				if (error==-1){
					printf("failed to draw victory\n");
					return -1;
				}
			}
			return 0;
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
			error=set_victory_control(i,i,gamePanel,player);
			if (error==-1){
				printf("failed to draw victory\n");
				return -1;
			}
		}
		return 0;
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
			error=set_victory_control(i,TIC_TAC_TOE_ROWS-i-1,gamePanel,player);
			if (error==-1){
				printf("failed to draw victory\n");
				return -1;
			}
		}
		return 0;
	}
	printf("ERROR: color_ttc was called but there is no victory\n");
	return -2;
}

/* function accepts TTC board matrix coordinates, 
 * locates the relevant TTC image, and creates 
 * a striked child underneath*/
int set_victory_control(int i,int j,element_cntrl game_panel,int player){
	element_cntrl ttc_button;
	int k=0;

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
		if(ttc_button==NULL){
			printf("ERROR: failed to find \"piece of victory\" at row %d and colunm %d in set_victory_control(ttc)\n",i,j);
			return -1;
		}
	}

	/*Choose Striked X or O*/
	if (player==1){
		// free previous control
		free_control(ttc_button->cntrl);
		buttomNum--;

		ttc_button->cntrl=new_button(ttc_button->cntrl->x,
			ttc_button->cntrl->y,TTC_BTNXPATH_VICTORY,1,NULL,0);
		if(ttc_button==NULL){
			printf("ERROR: failed to make button for \"piece of victory\" at row %d and colunm %d in set_victory_control(ttc)\n",i,j);
			return -1;
		}
	}
	else {
		// free previous control
		free_control(ttc_button->cntrl);
		buttomNum--;

		ttc_button->cntrl=new_button(ttc_button->cntrl->x,
			ttc_button->cntrl->y,TTC_BTNOPATH_VICTORY,1,NULL,0); 
		if(ttc_button==NULL){
			printf("ERROR: failed to make button for \"piece of victory\" at row %d and colunm %d in set_victory_control(ttc)\n",i,j);
			return -1;
		}
	}
	return 0;
}
