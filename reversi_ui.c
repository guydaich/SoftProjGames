#include "reversi_ui.h"

int rv_is_victory(int* game_state);

element_cntrl	rv_panel_function(int* game_state,int  (*makeMove)(int *quit,SDL_Event* test_event))
{
	control *rv_grid=NULL;
	control *rv_button=NULL;
	element_cntrl root=NULL, grid=NULL, temp=NULL, children_panel=NULL;
	linked_list_cntrl list=NULL;
	int i=0,j=0,error=0;
 	
	/*create root panel and grid as a child*/
	root = new_control_element(new_panel(0,0,600,600,255,255,255,1));
	if (root==NULL){
		printf("can't make root in rv_panel_function\n");
		return NULL;
	}
	list = new_control_list();
	if (list==NULL){
		printf("can't make list for root in rv_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	rv_grid = new_button(0,0,RVR_GRIDPATH,1,"",1);
	if (rv_grid==NULL){
        printf("can't make rv_grid in rv_panel_function\n");
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	rv_grid->pressedButton=makeMove;
	grid = new_control_element(rv_grid);
	if (grid==NULL){
		printf("can't make control element for rv_grid in rv_panel_function\n");
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	add_control_element_to_list(list,grid);
	set_list_as_children(list,root);
	
	/*panel to organise children within the game board*/
	children_panel = new_control_element(new_panel(49,49,598,598,255,255,255,0));
	if (children_panel==NULL){
		printf("can't make children_panel in rv_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	list = new_control_list();
	if (list==NULL){
		printf("can't make list for rv_grid in rv_panel_function\n");
		free_control(children_panel->cntrl);
		free(children_panel);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	add_control_element_to_list(list,children_panel);
	set_list_as_children(list,grid);

	/*create grid children - the board pieces*/
	list = new_control_list();
	if (list==NULL){
		printf("can't make list for children_panel in rv_panel_function\n");
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
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
                    if (temp==NULL){
					printf("can't make control element for piece in rv_panel_function\n");
					error=1;
					break;
				}
				add_control_element_to_list(list,temp);
			}
			else if(game_state[i*REVERSI_ROWS + j]==REVERSI_PLAYER_1 || game_state[i*REVERSI_ROWS + j] == REVERSI_PLAYER_2){
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

int color_rv(int* game_state,int player,element_cntrl ui_tree){
	element_cntrl gameUiBoard;
	int victoriusPlayer=0,error=0;

	gameUiBoard=ui_tree->children->tail->children->head->children->head;
	victoriusPlayer=rv_is_victory(game_state);
	if (victoriusPlayer==REVERSI_PLAYER_1){
		error=newButtonGeneric(gameUiBoard->children,25,25,"Player 1 won!",emptryButton,0);
		if(error==-1){
			printf("ERROR: failed to make button for victory in color_rv\n");
			return -1;
		}
	}
	else if (victoriusPlayer==REVERSI_PLAYER_2){
		error=newButtonGeneric(gameUiBoard->children,25,25,"Player 2 won!",emptryButton,0);
		if(error==-1){
			printf("ERROR: failed to make button for victory in color_rv\n");
			return -1;
		}
	}
	else {
		printf("ERROR: color_rv was called but there is no victory\n");
		return -2;
	}
	gameUiBoard->children->tail->parent=gameUiBoard;
	return 0;
}
