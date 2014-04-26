#include "windowsDrawing.h"

extern int quit;
extern int gameNum;
extern int controlElementNum;
extern int buttomNum;
extern int panelNum;
extern int windowNum;

game* cur_game=NULL;
element_cntrl ui_tree=NULL;

#define GAME_AREA_W 700
#define GAME_AREA_H 1000
#define GAME_PANEL_W 300
#define GAME_PANEL_H 1000
#define BTN_X_OFFSET 25
#define BTN_Y_OFFSET 20
#define BTN_H 100
#define PADDING 10
#define PLAYER_LABEL_X_OFFSET 160
#define PLAYER_LABEL_X_PADDING 300
#define PLAYER_LABEL_Y_OFFSET 2
#define PLAYER_LABEL_H 30
#define PLAYER_LABEL_W 100
#define BTN_PANEL_X 0
#define BTN_PANEL_Y 0
#define BTN_PANEL_W 250
#define BTN_PANEL_H 60
#define BTN_PANEL_R 220
#define BTN_PANEL_G 220
#define BTN_PANEL_B 220
#define BTN_X 20
#define BTN_Y 20
#define BTN_X_PAD 50
#define MAIN_X 0
#define MAIN_Y 0
#define MAIN_W 240
#define MAIN_H 300
#define CHOICE_WIN_X 0
#define CHOICE_WIN_Y 0
#define CHOICE_WIN_W 250
#define CHOICE_WIN_H 60
#define START_PANEL_X 0
#define START_PANEL_Y 0
#define START_PANEL_W 240
#define START_PANEL_H 300
#define START_BTN_X 20
#define START_BTN_Y 20
#define START_BTN_H 60	
#define QUESTION_X 0
#define QUESTION_Y 0
#define QUESTION_W 500
#define QUESTION_H 250
#define TEXTAREA_PATH "./gfx/textArea.bmp"
#define NOTIF_X 170
#define NOTIF_Y 30
#define NOTIF_PAD 160

#define BUTTON_LABEL_PATH "./gfx/button_label.bmp"

/* create a defualt tree for non-game area */
int get_default_ui_tree()
{
	int error=0, i=0;
	element_cntrl root;
	control* temp_control;
	linked_list_cntrl list; 

	char* captions[7] = {RESTART,SAVE,PAUSE,MAIN_MENU,DIFFP1,DIFFP2,QUIT};
	int (*functions[7])(int* a, SDL_Event* b)  = {restart_game,go_to_save_menu,set_unpause,go_to_start_menu,go_to_difficulty_player1,go_to_difficulty_player2,quit_game};
	
	root = new_control_element(new_window(0,0,GAME_AREA_H,GAME_AREA_W));
	if (root==NULL){
		return -1;
	}
	/*create panel children*/	
	list = new_control_list();
	if (list==NULL){
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	/*button panel*/
	temp_control = new_panel(GAME_AREA_W,0,GAME_PANEL_H,GAME_AREA_H,BTN_PANEL_R,BTN_PANEL_G,BTN_PANEL_B,1);
	if (temp_control==NULL){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	error=add_control_to_element_list(temp_control,list);
	if (error<0){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	set_list_as_children(list,root);

	/*panel children*/
	list = new_control_list();
	if (list==NULL){
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	/*label - paint first*/
	/*temp_control = new_label(0,0,0,0,BUTTON_LABEL_PATH,255,0,255,1,"");
	if (temp_control==NULL){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	error=add_control_to_element_list(temp_control,list);
	if (error<0){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}*/


	for (i=0; i < 7; i++)
	{
		error=new_generic_button(list,BTN_X_OFFSET,BTN_Y_OFFSET + BTN_H*i + PADDING,captions[i],functions[i],0);
		if (error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
		}
	}

	/* set buttons as panel children*/
	set_list_as_children(list,root->children->head);
	ui_tree=root;
	return 0;
}

/* used to open all windows, except for game window and notification widows
 * most windows (difficulties, saves, loads, player types) are generic*/
int run_window(user_selection_type selection_window_type){
	element_cntrl pressed_Button=NULL;
	int user_selection,number_choices=1,error=0;
	SDL_Event test_event; 
	int (*buttonAction)(int *choise,SDL_Event* test_event)=empty_click_handle;
	char** captionArray;
	
	/*clean up existing ui*/
	if (ui_tree!=NULL){
		free_control_list(ui_tree);
		ui_tree=NULL;
	}

	/* the main menu isn't generic in it's types of buttons, so handeled differently*/
	if (selection_window_type==SELECT_MAIN_MENU){
		ui_tree=start_window();
		if (ui_tree==NULL){
			if (cur_game!=NULL){
				free(cur_game->board);
				free(cur_game);
				gameNum--;
			}
			return -1;
		}
	}

	/*empty implemamtation where neccassry */
	else if(selection_window_type==EMPTY_WINDOW){
		return 0;
	}
	/*all other menues*/
	else {
		captionArray=init_choice_window(&buttonAction,&number_choices,selection_window_type);
		if (captionArray==NULL){
			if (cur_game!=NULL){
				free(cur_game->board);
				free(cur_game);
				gameNum--;
				cur_game=NULL;
			}
			return -1;
		}
		ui_tree=choice_window(number_choices,buttonAction,captionArray);
		if (ui_tree==NULL){
			if (cur_game!=NULL){
				free(cur_game->board);
				free(cur_game);
				gameNum--;
				cur_game=NULL;
			}
			return -1;
		}
	}
	if (!quit){
		if (draw_ui_tree(ui_tree) < 0)
		{
			printf("ERROR: drawing UI tree Failed");
			return -1;
		}
		SDL_Flip( ui_tree->cntrl->srfc );
	}
	
	/*enter main SDL loop within current window, 
	break loop on selection of a button with a handler*/
	while(!quit)
    {
	while(SDL_PollEvent(&test_event)) {
		if(test_event.type == SDL_QUIT){
			quit=1;
			//free_control_list(ui_tree);
			break;
		}
		switch(test_event.type) {
		 case SDL_MOUSEBUTTONDOWN:
			 find_element_by_coordinates(ui_tree,test_event.button.x,test_event.button.y,&pressed_Button);
			 if(pressed_Button==NULL)
			 {
				 break;
			 }
			 user_selection=pressed_Button->cntrl->button_choice;
			 error=pressed_Button->cntrl->pressed_button(&user_selection,&test_event);
			 if (error<0){
				free_control_list(ui_tree);
				ui_tree=NULL;
				if (cur_game!=NULL){
					free(cur_game->board);
					free(cur_game);
					gameNum--;
					cur_game=NULL;
				}
				return -1;
			 }
			 return 0;
			 break;
		 default: //unhandled event
			 break;
		 }
	}
	}
	return 0;
}

/* the last window to be drawn before game window, that is responsible 
 * for initialising the ui tree before the game menu: 
 * usually a dificulty or empty window */
int game_init(user_selection_type selection_window_type)
{
	int error;
	if (ui_tree!=NULL){
		free_control_list(ui_tree);
		ui_tree=NULL;
	}
	error=run_window(selection_window_type);
	if (error<0){
		return -1;
	}
	else{
		free_control_list(ui_tree);
		ui_tree=NULL;
	}
	if (cur_game==NULL){
		return 0;
	}
	error=get_default_ui_tree();
	if (error<0){
		return -1;
	}
	error=draw_game();
	if (error<0){
		free_control_list(ui_tree);
		ui_tree=NULL;
		free(cur_game->board);
		free(cur_game);//checked for null before
		gameNum--;
		return -1;
	}
	return 0;
}

/*adds the player labels to the implemented game's ui tree, and draws*/
int draw_game ()
{
	control *playerLabel_control=NULL;
	element_cntrl game_panel=NULL,temp_elem=NULL;
	char *player_captions[2]={P1,P2};
	char *player_sources[2];
	int i=0;

	clear_game_panel(ui_tree);
	game_panel = (cur_game)->panel_function((cur_game)->board,handle_next_move);
	if (game_panel==NULL){
		return -1;
	}
	//label - player
	if (cur_game->cur_player==-1){
		player_sources[0]=P_EMPTY;
		player_sources[1]=P_COLORED;
	}
	else{
		player_sources[0]=P_COLORED;
		player_sources[1]=P_EMPTY;
	}
	for (i=0;i<2;i++){
		playerLabel_control = new_label(PLAYER_LABEL_X_OFFSET+PLAYER_LABEL_X_PADDING*i,PLAYER_LABEL_Y_OFFSET,
			PLAYER_LABEL_W,PLAYER_LABEL_H,player_sources[i],255,0,255,1,player_captions[i]);
		if (playerLabel_control==NULL){
			return -1;
		}
		temp_elem = new_control_element(playerLabel_control);
		if (temp_elem==NULL){
			return -1;
		}
		temp_elem->parent=game_panel;
		add_control_element_to_list(game_panel->children,temp_elem);
	}

	add_control_element_to_list(ui_tree->children,game_panel);
	game_panel->parent= ui_tree;
	if (draw_ui_tree(ui_tree)<0)
	{
		printf("ERROR: drawing UI Tree Failed");
		return -1;
	}
	SDL_Flip( (ui_tree->cntrl->srfc) );
	return 0;
}

/*a generic construction of multiple choice windows: difficulty, save, load,etc*/
element_cntrl choice_window(int number_choices,int (*buttonAction)(int *choise,SDL_Event* test_event),char** captionStart){
	element_cntrl root, temp_elem;
	control* temp_control;
	linked_list_cntrl list;
	int i,error=0;
	
	root = new_control_element(new_window(CHOICE_WIN_X,CHOICE_WIN_Y,CHOICE_WIN_W,(number_choices+2)*CHOICE_WIN_H));
	if (root==NULL){
		return NULL;
	}
	/*create panel children*/	
	list = new_control_list();
	if (list==NULL){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}

	/*button panel*/
	temp_control = new_panel(BTN_PANEL_X,BTN_PANEL_Y,BTN_PANEL_W,
		(number_choices+2)*BTN_PANEL_H,BTN_PANEL_R,BTN_PANEL_G,BTN_PANEL_B,1);

	if (temp_control==NULL){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	temp_elem = new_control_element(temp_control);
	if (temp_elem==NULL){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	add_control_element_to_list(list,temp_elem);
	set_list_as_children(list,root);

	/*panel children*/
	list = new_control_list();
	if (list==NULL){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}


	for (i=1;i<=number_choices;i++)
	{
		error=new_generic_button(list,BTN_X,BTN_Y+(i-1)*BTN_X_PAD,captionStart[i-1],buttonAction,i);
		free(captionStart[i-1]);
		if (error<0){
			break;
		}
	}
	if (error<0){
		//new_generic_button frees list 
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	free(captionStart);

	/*cancel is different than other selections*/
	error=new_generic_button(list,BTN_X,BTN_Y+(i-1)*BTN_X_PAD,CANCEL,go_to_start_menu,0);
	if (error<0){
		//new_generic_button frees list 
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	set_list_as_children(list,root->children->head);
	return root;

}

/*main menu init*/
element_cntrl start_window(){
	element_cntrl root;
	control* temp_control;
	linked_list_cntrl list; 
	int error=0, i=0;
	
	char* captions[3] = {CHOOSE_GAME,LOAD,CANCEL};
	int (*functions[3])(int* a, SDL_Event* b)  = {go_to_game_selection,go_to_load_menu,quit_game};

	root = new_control_element(new_window(MAIN_X,MAIN_Y,MAIN_W,MAIN_H));
	if(root==NULL){
		return NULL;
	}
	/*create panel children*/	
	list = new_control_list();
	if(list==NULL){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	/*button panel*/

	temp_control = new_panel(START_PANEL_X,START_PANEL_Y,START_PANEL_W,
							START_PANEL_H,BTN_PANEL_R,BTN_PANEL_G,BTN_PANEL_B,1);
	if(temp_control==NULL){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	error=add_control_to_element_list(temp_control,list);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	set_list_as_children(list,root);

	/*panel children*/
	list = new_control_list();
	if (list==NULL){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}

	for (i=0; i< 3; i++)
	{
		error=new_generic_button(list,START_BTN_X,START_BTN_Y + i*START_BTN_H,captions[i],functions[i],0);
		if (error<0){
			free_control(root->cntrl);
			free(root);
			return NULL;
		}

	}
	
	set_list_as_children(list,root->children->head);
	return root;
}

/* cosntruct paramters for generic menu construction function*/
char** init_choice_window(int (**pressed_button)(int *quit,SDL_Event* test_event),int *number_choices,user_selection_type flag){
	char** captionArray=NULL;
	char* buttonName;
	int i,error=0;
	char *transporter;

	if (flag==SELECT_GAME){
		*pressed_button=set_current_game;
		*number_choices=3;
		captionArray=(char**)calloc(*number_choices,sizeof(char*));
		captionArray[0]=(char *)calloc(strlen(ttc_get_name())+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[0],ttc_get_name());
		captionArray[1]=(char *)calloc(strlen(REVERSI_NAME)+1,sizeof(char));
		strcpy(captionArray[1],REVERSI_NAME);
		captionArray[2]=(char *)calloc(strlen(Connect4_NAME)+1,sizeof(char));
		strcpy(captionArray[2],Connect4_NAME);
		return captionArray;
	}
	else if (flag==SELECT_PLAYER_TYPES){
		*pressed_button=set_game_player_types;
		*number_choices=4;
		captionArray=(char**)calloc(*number_choices,sizeof(char*));
		captionArray[0]=(char *)calloc(strlen(AI_1)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[0],AI_1);
		captionArray[1]=(char *)calloc(strlen(AI_2)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[1],AI_2);
		captionArray[2]=(char *)calloc(strlen(AI_3)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[2],AI_3);
		captionArray[3]=(char *)calloc(strlen(AI_4)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[3],AI_4);
		return captionArray;
	}
	else if (flag==LOAD_GAME){
		*pressed_button=load_game;
		*number_choices=5;
		transporter=(char *)LOAD_SLOT;
		captionArray=(char**)calloc(*number_choices,sizeof(char*));
		if (captionArray==NULL){
			return NULL;
		}
	}
	else if (flag==SAVE_GAME){
		*pressed_button=save_game;
		*number_choices=5;
		transporter=(char *)SAVE_SLOT;
		captionArray=(char**)calloc(*number_choices,sizeof(char*));
		if (captionArray==NULL){
			return NULL;
		}
	}
	else if (flag==SET_DIFF_PLAYER1){
		if (cur_game==NULL){
			return NULL;
		}
		*pressed_button=set_player1_difficulty;
		*number_choices=cur_game->difficulty_num;
		transporter=(char *)DIFFICULTY;
		captionArray=(char**)calloc(*number_choices,sizeof(char*));
		if (captionArray==NULL){
			return NULL;
		}
	}
	 else if (flag==SET_DIFF_PLAYER2){
		if (cur_game==NULL){
			return NULL;
		}
		*pressed_button=set_player2_difficulty;
		*number_choices=cur_game->difficulty_num;
		transporter=(char *)DIFFICULTY;
		captionArray=(char**)calloc(*number_choices,sizeof(char*));
		if (captionArray==NULL){
			return NULL;
		}
	}
		for (i=0;i<*number_choices;i++){
		captionArray[i]=transporter;
			buttonName=(char*)malloc(strlen(captionArray[i])+3);//for the meanwhile a leak; TODO: leak??
			if (buttonName==NULL){
				error =-1;
				break;
			}
			sprintf(buttonName,"%s %d",captionArray[i],i);
			captionArray[i]=buttonName;
		}
		if (error ==-1){
			for (;i>=0;i--){
				free(captionArray[i]);
			}
			free(captionArray);
			return NULL;
		}
	return captionArray;
}

/* notification window - asking for user input or notifying*/
int question_window(char *qustion,notification_type flag){
	element_cntrl pressed_Button=NULL;
	int user_selection,error;
	SDL_Event test_event;

	element_cntrl root;
	control* temp_control;
	linked_list_cntrl list; 

	if (ui_tree!=NULL){
		free_control_list(ui_tree);
		ui_tree=NULL;
	}

	root = new_control_element(new_window(QUESTION_X,QUESTION_Y,QUESTION_W,QUESTION_H));
	if(root==NULL){
		return -1;
	}
	/*create panel children*/	
	list = new_control_list();
	if(list==NULL){
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	/*button panel*/
	temp_control = new_panel(QUESTION_X,QUESTION_Y,QUESTION_W,QUESTION_H,255,255,255,1);
	if(temp_control==NULL){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	error=add_control_to_element_list(temp_control,list);
	if(error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	//label - paint first
	temp_control = new_label(QUESTION_X,QUESTION_Y,QUESTION_W,QUESTION_H,TEXTAREA_PATH,255,0,255,1,qustion);
	if(temp_control==NULL){
		free_detached_list(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	error=add_control_to_element_list(temp_control,list);
	if(error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	set_list_as_children(list,root);
	ui_tree=root;
	error=notfication_window(flag);
	if(error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	if (draw_ui_tree(ui_tree) < 0)
	{
		printf("ERROR: drawing UI tree Failed");
		return -1;
	}
	SDL_Flip( ui_tree->cntrl->srfc );
	
	/*sdl loop until button selection*/
	while(!quit)
    {
	while(SDL_PollEvent(&test_event)) {
		if(test_event.type == SDL_QUIT){
			quit=1;
			//free_control_list(ui_tree);
			break;
		}
		switch(test_event.type) {
		 case SDL_MOUSEBUTTONDOWN:
			 /*retrives the selection encoded within the button, doesn't use the function*/
			 find_element_by_coordinates(ui_tree,test_event.button.x,test_event.button.y,&pressed_Button);
			 if(pressed_Button==NULL)
			 {
				 break;
			 }
			 user_selection=pressed_Button->cntrl->button_choice;
			 return user_selection;
		}
	}
	}
	return 0;
}

/* constructs parameter for question and notification window*/
int notfication_window(notification_type flag){
	linked_list_cntrl list=NULL;
	int error=0;

	list = new_control_list();
	if(list==NULL){
		return -1;
	}
	if(flag==USER_PROMPT){
		error=new_generic_button(list,NOTIF_X,NOTIF_Y,YES,empty_click_handle,1);
		if (error<0){
			return -1;
		}
		error=new_generic_button(list,NOTIF_X,NOTIF_Y+NOTIF_PAD,NO,empty_click_handle,0);
		if (error<0){
			return -1;
		}
	}
	else if(flag==USER_NOTIF){
		error=new_generic_button(list,NOTIF_X,NOTIF_Y+NOTIF_PAD,OK,empty_click_handle,0);
		if (error<0){
			return -1;
		}
	}
	set_list_as_children(list,ui_tree->children->tail);
	return 0;
}
