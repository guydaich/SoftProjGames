#include "connect4_ui.h"

extern int buttomNum;

element_cntrl	C4_panel_function(int* game_state,int  (*handle_next_move)(int *quit,SDL_Event* test_event))
{
	control *C4_grid=NULL;
	control *C4_button=NULL;
	element_cntrl root=NULL, grid=NULL, temp=NULL, children_panel=NULL;
	linked_list_cntrl list=NULL;
	int i=0,j=0,error=0;

	root = new_control_element(new_panel(0,0,C4_WGRID,C4_HGRID,255,255,255,1));
	if (root==NULL){
		printf("can't make root in C4_panel_function\n");
		return NULL;
	}
	/*create panel children*/	
	list = new_control_list();
	if (list==NULL){
		printf("can't make list for root in C4_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	// grid surface - create control and element
	C4_grid = new_button(0,0,C4_GRIDPATH,0,NULL,1);
	if (C4_grid==NULL){
	        printf("can't make C4_grid in C4_panel_function\n");
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	C4_grid->pressed_button=handle_next_move;
	grid = new_control_element(C4_grid);
	if (grid==NULL){
		printf("can't make control element for C4_grid in C4_panel_function\n");
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	// add grid to children list
	add_control_element_to_list(list,grid);
	// update root children, and grid parent
	set_list_as_children(list,root);
	
	children_panel = new_control_element(new_panel(C4_XOFFSET,C4_YOFFSET,600,600,255,255,255,0));
	if (children_panel==NULL){
		printf("can't make children_panel in C4_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	list = new_control_list();
	if (list==NULL){
		printf("can't make list for C4_grid in C4_panel_function\n");
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
		printf("can't make list for children_panel in C4_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
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
				if (temp==NULL){
					printf("can't make control element for piece in C4_panel_function\n");
					error=1;
					break;
				}
				add_control_element_to_list(list,temp);
			}
			else if(game_state[i*CONNECT4_COLS + j]==CONNECT4_PLAYER_1 || game_state[i*CONNECT4_COLS + j] == CONNECT4_PLAYER_2){
				printf("can't make piece in C4_panel_function\n");
				error=1;
				break;
			}
		}
		if(error==1){
			break;
		}
	}
	if(error==1){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	//update parent-children
	set_list_as_children(list,children_panel);

	return root;
}


int color_c4(int* game_state,int player,element_cntrl ui_tree){
	element_cntrl gamePanel=NULL;
	int i=0, j=0, k=0, lineScore = 0,error=0;;

	gamePanel=ui_tree->children->tail;

	//lines in rows calculation
	for (i = 0; i < CONNECT4_ROWS; i++) {
		for (j = 0; j < CONNECT4_COLS - 3; j++) {
			lineScore = 0;
			for (k = 0; k < 4; k++) {
				if (game_state[(i)*CONNECT4_COLS + j + k] == CONNECT4_PLAYER_1){
					//CONNECT4_COLS and not rows because move next line==move on all colunms of perv
					lineScore++;
				}
				else if (game_state[(i)*CONNECT4_COLS + j + k] == CONNECT4_PLAYER_2){
					lineScore--;
				}
			}
			if (lineScore == 4){
				for (k = 0; k < 4; k++){
					error=c4_set_victory_control(i,j+k,gamePanel,CONNECT4_PLAYER_1);
					if(error==-1){
						printf("failed to draw victory\n");
						return -1;
					}
				}
				return 0;
			}
			else if (lineScore==-4){
				for (k = 0; k < 4; k++){
					error=c4_set_victory_control(i,j+k,gamePanel,CONNECT4_PLAYER_2);
					if(error==-1){
						printf("failed to draw victory\n");
						return -1;
					}
				}
				return 0;
			}
		}
	}

	//lines in clonums calculation
	for (j = 0; j < CONNECT4_COLS; j++) {
		for (i = 0; i < CONNECT4_ROWS - 3; i++) {
			lineScore = 0;
			for (k = 0; k < 4; k++) {
				if (game_state[(i + k)*CONNECT4_COLS + j] == CONNECT4_PLAYER_1){//is it really HUMAN????
					lineScore++;
				}
				else if (game_state[(i + k)*CONNECT4_COLS + j] == CONNECT4_PLAYER_2){
					lineScore--;
				}
			}
			if (lineScore == 4){
				for (k = 0; k < 4; k++){
					error=c4_set_victory_control(i+k,j,gamePanel,CONNECT4_PLAYER_1);
					if(error==-1){
						printf("failed to draw victory\n");
						return -1;
					}
				}
				return 0;
			}
			else if (lineScore==-4){
				for (k = 0; k < 4; k++){
					error=c4_set_victory_control(i+k,j,gamePanel,CONNECT4_PLAYER_2);
					if(error==-1){
						printf("failed to draw victory\n");
						return -1;
					}
				}
				return 0;
			}
		}
	}

	//lines in diaganals calculation
	for (j = 0; j < CONNECT4_COLS; j++) {
		for (i = 0; i < CONNECT4_ROWS; i++) {
			lineScore = 0;
			if (((i + 3) < CONNECT4_ROWS) && ((j + 3) < CONNECT4_COLS)){//diganals which go up and right
				for (k = 0; k < 4; k++) {
					if (game_state[(i + k)*CONNECT4_COLS + j + k] == CONNECT4_PLAYER_1){
						lineScore++;
					}
					else if (game_state[(i + k)*CONNECT4_COLS + j + k] == CONNECT4_PLAYER_2){
						lineScore--;
					}
				}
				if (lineScore == 4){
					for (k = 0; k < 4; k++){
						error=c4_set_victory_control(i+k,j+k,gamePanel,CONNECT4_PLAYER_1);
						if(error==-1){
			                printf("failed to draw victory\n");
							return -1;
						}
					}
					return 0;
				}
				else if (lineScore==-4){
					for (k = 0; k < 4; k++){
						error=c4_set_victory_control(i+k,j+k,gamePanel,CONNECT4_PLAYER_2);
						if(error==-1){
			                printf("failed to draw victory\n");
							return -1;
						}
					}
					return 0;
				}
			}
			lineScore = 0;
			if (((i - 3) > -1) && ((j + 3) < CONNECT4_COLS)){//diganals which go down and right
				for (k = 0; k < 4; k++) {
					if (game_state[(i - k)*CONNECT4_COLS + j + k] == CONNECT4_PLAYER_1){
						lineScore++;
					}
					else if (game_state[(i - k)*CONNECT4_COLS + j + k] == CONNECT4_PLAYER_2){
						lineScore--;
					}
				}
				if (lineScore == 4){
					for (k = 0; k < 4; k++){
						error=c4_set_victory_control(i-k,j+k,gamePanel,CONNECT4_PLAYER_1);
						if(error==-1){
			                printf("failed to draw victory\n");
							return -1;
						}
					}
					return 0;
				}
				else if (lineScore==-4){
					for (k = 0; k < 4; k++){
						error=c4_set_victory_control(i-k,j+k,gamePanel,CONNECT4_PLAYER_2);
						if(error==-1){
							printf("failed to draw victory\n");
							return -1;
						}
					}
					return 0;
				}
			}
		}
	}
	printf("ERROR: color_c4 was called but there is no victory\n");
	return -2;
}



/* function accepts Connect4 board matrix coordinates, 
 * locates the relevant Connect4 image, and creates 
 * a striked child underneath*/
int c4_set_victory_control(int i,int j,element_cntrl game_panel,int player)
{
	element_cntrl c4_button;
	int k;

	/* TTC pieces in ui tree */
	c4_button=game_panel->children->head->children->head->children->head;
	/*advance in grid, until reaching piece i,j*/
	
	for(k=0; k < (CONNECT4_ROWS-1)*CONNECT4_COLS+CONNECT4_COLS-1; k++)
	{	
		if(c4_button->cntrl->x==j*C4_WBTN 
			&& c4_button->cntrl->y==i*C4_HBTN) {
			break;
		}
		if (c4_button->next != NULL)
			c4_button=c4_button->next;
		else{
			printf("ERROR: failed to find \"piece of victory\" at row %d and colunm %d in c4_set_victory_control\n",i,j);
			return -1;
		}
	}

	/*Choose Striked X or O*/
	if (player==1){
		// free previous control
		free_control(c4_button->cntrl); 
		buttomNum--;

		c4_button->cntrl=new_button(c4_button->cntrl->x,
			c4_button->cntrl->y,C4_BTN_RED_VICTORY_PATH,1,NULL,0);
		if(c4_button==NULL){
			printf("ERROR: failed to make button for \"piece of victory\" at row %d and colunm %d in c4_set_victory_control\n",i,j);
			return -1;
		}
	}
	else {
		// free previous control
		free_control(c4_button->cntrl); 
		buttomNum--;

		c4_button->cntrl=new_button(c4_button->cntrl->x,
			c4_button->cntrl->y,C4_BTN_BLUE_VICTORY_PATH,1,NULL,0);
		if(c4_button==NULL){
			printf("ERROR: failed to make button for \"piece of victory\" at row %d and colunm %d in c4_set_victory_control\n",i,j);
			return -1;
		}
	}
	return 0;
}
