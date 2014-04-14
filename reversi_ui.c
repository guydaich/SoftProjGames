#include "reversi_ui.h"
#include "controls.h"

int rv_is_victory(int* game_state);

element_cntrl	rv_panel_function(int* game_state,int  (*makeMove)(int *quit,SDL_Event* test_event))
{
	control *rv_grid;
	control *rv_button;
	element_cntrl root, grid, temp, children_panel;
	linked_list_cntrl list;
	int i,j,error=0;
 	
	/*create root panel and grid as a child*/
	root = new_control_element(new_panel(0,0,600,600,255,255,255,1));
	if (root==NULL){
		return NULL;
	}
	list = new_control_list();
	if (list==NULL){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	rv_grid = new_button(0,0,RVR_GRIDPATH,1,"",1);
	if (rv_grid==NULL){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	rv_grid->pressedButton=makeMove;
	grid = new_control_element(rv_grid);
	if (grid==NULL){
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
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	list = new_control_list();
	if (list==NULL){
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
	int victoriusPlayer,error;

	gameUiBoard=ui_tree->children->tail->children->head->children->head;
	victoriusPlayer=rv_is_victory(game_state);
	if (victoriusPlayer==REVERSI_PLAYER_1){
		error=newButtonGeneric(gameUiBoard->children,25,25,"Player 1 won!",emptryButton,0);
		if(error==-1){
			return -1;
		}
	}
	else{
		error=newButtonGeneric(gameUiBoard->children,25,25,"Player 2 won!",emptryButton,0);
		if(error==-1){
			return -1;
		}
	}
	gameUiBoard->children->tail->parent=gameUiBoard;
	return 0;
}
