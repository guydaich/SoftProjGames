#include "connect4_ui.h"

element_cntrl	C4_panel_function(int* game_state,void  (*makeMove)(int *quit,SDL_Event* test_event))
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


void color_c4(int* game_state,int player,element_cntrl ui_tree){
	element_cntrl gamePanel;
	int i, j, k, lineScore = 0, totalScore = 0;

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
					c4_set_victory_control(i,j+k,gamePanel,CONNECT4_PLAYER_1);
				}
				return;
			}
			else if (lineScore==-4){
				for (k = 0; k < 4; k++){
					c4_set_victory_control(i,j+k,gamePanel,CONNECT4_PLAYER_2);
				}
				return;
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
					c4_set_victory_control(i+k,j,gamePanel,CONNECT4_PLAYER_1);
				}
				return;
			}
			else if (lineScore==-4){
				for (k = 0; k < 4; k++){
					c4_set_victory_control(i+k,j,gamePanel,CONNECT4_PLAYER_2);
				}
				return;
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
						c4_set_victory_control(i+k,j+k,gamePanel,CONNECT4_PLAYER_1);
					}
					return;
				}
				else if (lineScore==-4){
					for (k = 0; k < 4; k++){
						c4_set_victory_control(i+k,j+k,gamePanel,CONNECT4_PLAYER_2);
					}
					return;
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
						c4_set_victory_control(i-k,j+k,gamePanel,CONNECT4_PLAYER_1);
					}
					return;
				}
				else if (lineScore==-4){
					for (k = 0; k < 4; k++){
						c4_set_victory_control(i-k,j+k,gamePanel,CONNECT4_PLAYER_2);
					}
					return;
				}
			}
		}
	}

}



/* function accepts Connect4 board matrix coordinates, 
 * locates the relevant Connect4 image, and creates 
 * a striked child underneath*/
void c4_set_victory_control(int i,int j,element_cntrl game_panel,int player)
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
		else
			return;
	}

	/*Choose Striked X or O*/
	if (player==1){
		c4_button->cntrl=new_button(c4_button->cntrl->x,
			c4_button->cntrl->y,C4_BTN_RED_VICTORY_PATH,1,NULL,0);
		// free previous control
		free_control(c4_button->cntrl); 
	}
	else {
		c4_button->cntrl=new_button(c4_button->cntrl->x,
			c4_button->cntrl->y,C4_BTN_BLUE_VICTORY_PATH,1,NULL,0);
		// free previous control
		free_control(c4_button->cntrl); 
	}
}
