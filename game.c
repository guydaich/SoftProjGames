#include "game.h"

int quit=0;
extern char* TIC_TAC_TOE_NAME;
extern char* REVERSI_NAME;
extern char* Connect4_NAME;
extern int gameNum;
extern int controlElementNum;
extern int buttomNum;
extern int panelNum;
extern int windowNum;
int isTwoComputers=0;


game* cur_game=NULL;
element_cntrl ui_tree=NULL;


int gui_init()
{
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
		printf("ERROR: unable to init SDL: %s\n",SDL_GetError());
		return -1;
	}

	if (TTF_Init() < 0){
		printf("ERROR: unable to init SDL: %s\n",SDL_GetError());
		return -1;
	}
	//freopen("CON","w",stdout);//make log files
	//freopen("CON","w",stderr);
	return 1;
}

int main( int argc, char* args[] )
{
	element_cntrl pressed_Button=NULL; 
	SDL_Event test_event; 
	int pause=0,error;

	gui_init();
	error=runStartManu(&pause,&test_event);
	if (error<0 || cur_game==NULL){
		return 0;
	}
	while(!quit)
    {
		while(SDL_PollEvent(&test_event)) {
			if(test_event.type == SDL_QUIT){
				quit=1;
				break;
			}
			switch(test_event.type) {
			case SDL_MOUSEBUTTONDOWN:
				find_element_by_coordinates(ui_tree,test_event.button.x,test_event.button.y,&pressed_Button);
				if(pressed_Button==NULL)
				{
					 break;
				}
				error=pressed_Button->cntrl->pressedButton(&pause,&test_event);
				if (error<0 ||cur_game==NULL){
					printf("a catchAble error occured\n");
					return 0;
				}
				break;
			default: //unhandled event
				break;
			}
			if (cur_game!=NULL && cur_game->is_multiplayer==1 && pause==0){
				error=handelAI_VS_AI(&pause);
				if (error<0 ||cur_game==NULL){
					printf("a catchAble error occured\n");
					freeControlList(ui_tree);
					if (cur_game!=NULL)
					{
						free(cur_game);
						gameNum--;
				}
					return 0;
				}
			}
		}
	}
	freeControlList(ui_tree);
	if (cur_game!=NULL)
	{
		free(cur_game);
		gameNum--;
	}
	return 0;
}

int get_default_ui_tree()
{
	int error;
	element_cntrl root;
	control* temp_control;
	linked_list_cntrl list; 
	
	root = new_control_element(new_window(0,0,1000,700));
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
	temp_control = new_panel(700,0,300,1000,255,255,255,1);
	if (temp_control==NULL){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	error=addNewControlToList(temp_control,list);
	if (error<0){
		freeUnconnectedList(list);
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
	temp_control = new_label(0,0,0,0,"./gfx/button_label.bmp",255,0,255,1,"this is a text");
	if (temp_control==NULL){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	error=addNewControlToList(temp_control,list);
	if (error<0){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	/*restart*/
	error=newButtonGeneric(list,25,20,RESTART,restartGame,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	/*save*/
	error=newButtonGeneric(list,25,120,SAVE,runsaveManu,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	/*pause- unpause*/
	error=newButtonGeneric(list,25,220,PAUSE,setUnpause,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	/*main menu*/
	error=newButtonGeneric(list,25,320,MAIN_MENU,runStartManu,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	/*difficulties menus*/
	error=newButtonGeneric(list,25,420,DIFFP1,runDiffManuP1,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	error=newButtonGeneric(list,25,520,DIFFP2,runDiffManuP2,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	/*quit*/
	error=newButtonGeneric(list,25,620,QUIT,quitGame,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	/* set buttons as panel children*/
	set_list_as_children(list,root->children->head);
	ui_tree=root;
	return 0;
}

//run window in which a game is chosen
int runWindow(choiseWindowSign mainORLoad){
	element_cntrl pressed_Button=NULL;
	int whichGame,iterationNum=1,error=0;
	SDL_Event test_event; 
	int (*buttonAction)(int *choise,SDL_Event* test_event)=emptryButton;
	char** captionArray;

	if (ui_tree!=NULL){
		freeControlList(ui_tree);
		ui_tree=NULL;
	}
	if (mainORLoad==START_SIGN){
		ui_tree=startWindow();
		if (ui_tree==NULL){
			if (cur_game!=NULL){
				free(cur_game);
				gameNum--;
			}
			return -1;
		}
	}
	else if(mainORLoad==ZERO_SIGN){
		return 0;
	}
	else {
		captionArray=initialazeChoiseWindow(&buttonAction,&iterationNum,mainORLoad);
		if (captionArray==NULL){
			if (cur_game!=NULL){
				free(cur_game);
				gameNum--;
				cur_game=NULL;
			}
			return -1;
		}
		ui_tree=choiseWindow(iterationNum,buttonAction,captionArray);
		if (ui_tree==NULL){
			if (cur_game!=NULL){
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
	
	while(!quit)
    {
	while(SDL_PollEvent(&test_event)) {
		if(test_event.type == SDL_QUIT){
			quit=1;
			//freeControlList(ui_tree);
			break;
		}
		switch(test_event.type) {
		 case SDL_MOUSEBUTTONDOWN:
			 find_element_by_coordinates(ui_tree,test_event.button.x,test_event.button.y,&pressed_Button);
			 if(pressed_Button==NULL)
			 {
				 break;
			 }
			 whichGame=pressed_Button->cntrl->buttonChoise;
			 error=pressed_Button->cntrl->pressedButton(&whichGame,&test_event);
			 if (error<0){
				freeControlList(ui_tree);
				ui_tree=NULL;
				if (cur_game!=NULL){
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

//go to main menu,choose game and initiate ui_tree
int game_init(choiseWindowSign mainORLoad)
{
	int error;
	if (ui_tree!=NULL){
		freeControlList(ui_tree);
		ui_tree=NULL;
	}
	error=runWindow(mainORLoad);
	if (error<0){
		return -1;
	}
	else{
		freeControlList(ui_tree);
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
		freeControlList(ui_tree);
		ui_tree=NULL;
		free(cur_game);//checked for null before
		gameNum--;
		return -1;
	}
	return 0;
}

//draw game and update ui_tree by cur_game
int draw_game ()
{
	control *playerLabel_control=NULL;
	element_cntrl game_panel=NULL,temp_elem=NULL;
	char *playerLabels[2]={P1,P2};
	char *playerSourses[2];
	int i=0;

	clear_game_panel(ui_tree);
	game_panel = (cur_game)->panel_function((cur_game)->board,makeMove);
	if (game_panel==NULL){
		return -1;
	}
	//label - player
	if (cur_game->cur_player==-1){
		playerSourses[0]=P_EMPTY;
		playerSourses[1]=P_COLORED;
	}
	else{
		playerSourses[0]=P_COLORED;
		playerSourses[1]=P_EMPTY;
	}
	for (i=0;i<2;i++){
		playerLabel_control = new_label(160+300*i,2,100,30,playerSourses[i],255,0,255,1,playerLabels[i]);
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

element_cntrl choiseWindow(int iterationNum,int (*buttonAction)(int *choise,SDL_Event* test_event),char** captionStart){
	element_cntrl root, temp_elem;
	control* temp_control;
	linked_list_cntrl list;
	int i,error=0;
	
	root = new_control_element(new_window(0,0,250,(iterationNum+2)*60));
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
	temp_control = new_panel(0,0,250,(iterationNum+2)*60,220,220,220,1);
	if (temp_control==NULL){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	temp_elem = new_control_element(temp_control);
	if (temp_elem==NULL){
		freeUnconnectedList(list);
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

	for (i=1;i<=iterationNum;i++)
	{
		error=newButtonGeneric(list,20,20+(i-1)*50,captionStart[i-1],buttonAction,i);
		free(captionStart[i-1]);
		if (error<0){
			break;
		}
	}
	if (error<0){
		//newButtonGeneric frees list 
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	free(captionStart);
	error=newButtonGeneric(list,20,20+(i-1)*50,CANCEL,runStartManu,0);
	if (error<0){
		//newButtonGeneric frees list 
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	set_list_as_children(list,root->children->head);
	return root;

}

element_cntrl startWindow(){
	element_cntrl root;
	control* temp_control;
	linked_list_cntrl list; 
	int error;

	root = new_control_element(new_window(0,0,240,300));
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
	temp_control = new_panel(0,0,240,300,220,220,220,1);
	if(temp_control==NULL){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	error=addNewControlToList(temp_control,list);
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

	error=newButtonGeneric(list,20,20,CHOOSE_GAME,goToMainMenu,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	error=newButtonGeneric(list,20,80,LOAD,runLoadManu,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}
	error=newButtonGeneric(list,20,160,CANCEL,quitGame,0);
	if (error<0){
		free_control(root->cntrl);
		free(root);
		return NULL;
	}

	set_list_as_children(list,root->children->head);
	return root;
}


char** initialazeChoiseWindow(int (**pressedButton)(int *quit,SDL_Event* test_event),int *iterationNum,choiseWindowSign flag){
	char** captionArray=NULL;
	char* buttonName;
	int i,error=0;

	if (flag==MAIN_SIGN){
		*pressedButton=chooseGame;
		*iterationNum=3;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		captionArray[0]=(char *)calloc(strlen(TIC_TAC_TOE_NAME)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[0],TIC_TAC_TOE_NAME);
		captionArray[1]=(char *)calloc(strlen(REVERSI_NAME)+1,sizeof(char));
		strcpy(captionArray[1],REVERSI_NAME);
		captionArray[2]=(char *)calloc(strlen(Connect4_NAME)+1,sizeof(char));
		strcpy(captionArray[2],Connect4_NAME);
	}
	else if (flag==LOAD_SIGN){
		*pressedButton=loadGame;
		*iterationNum=5;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		if (captionArray==NULL){
			return NULL;
		}
		for (i=0;i<*iterationNum;i++){
			captionArray[i]=(char *)LOAD_SLOT;
			buttonName=(char*)malloc(strlen(captionArray[i])+3);//for the meanwhile a leak;
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
	}
	else if (flag==SAVE_SIGN){
		*pressedButton=saveGame;
		*iterationNum=5;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		if (captionArray==NULL){
			return NULL;
		}
		for (i=0;i<*iterationNum;i++){
			captionArray[i]=(char *)SAVE_SLOT;
			buttonName=(char*)malloc(strlen(captionArray[i])+3);//for the meanwhile a leak;
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
	}
	else if (flag==AI_SIGN){
		*pressedButton=setmultiplayer;
		*iterationNum=4;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		captionArray[0]=(char *)calloc(strlen(AI_1)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[0],AI_1);
		captionArray[1]=(char *)calloc(strlen(AI_2)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[1],AI_2);
		captionArray[2]=(char *)calloc(strlen(AI_3)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[2],AI_3);
		captionArray[3]=(char *)calloc(strlen(AI_4)+1,sizeof(char));//alocate so we can free them freely
		strcpy(captionArray[3],AI_4);
	}
	else if (flag==DIFF1_SIGN){
		if (cur_game==NULL){
			return NULL;
		}
		*pressedButton=setDifficaltyP1;
		*iterationNum=cur_game->difficulty_num;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		if (captionArray==NULL){
			return NULL;
		}
		for (i=0;i<*iterationNum;i++){
			captionArray[i]=(char *)DIFFICALTY;
			buttonName=(char*)malloc(strlen(captionArray[i])+3);//for the meanwhile a leak;
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
	}
	 else if (flag==DIFF2_SIGN){
		if (cur_game==NULL){
			return NULL;
		}
		*pressedButton=setDifficaltyP2;
		*iterationNum=cur_game->difficulty_num;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		if (captionArray==NULL){
			return NULL;
		}
		for (i=0;i<*iterationNum;i++){
			captionArray[i]=(char *)DIFFICALTY;
			buttonName=(char*)malloc(strlen(captionArray[i])+3);//for the meanwhile a leak;
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
	 }
	return captionArray;
}

int askWindow(char *qustion,qustionWindowsSgin flag){
	element_cntrl pressed_Button=NULL;
	int whichGame,error;
	SDL_Event test_event;

	element_cntrl root;
	control* temp_control;
	linked_list_cntrl list; 

	if (ui_tree!=NULL){
		freeControlList(ui_tree);
		ui_tree=NULL;
	}

	root = new_control_element(new_window(0,0,500,250));
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
	temp_control = new_panel(0,0,500,250,255,255,255,1);
	if(temp_control==NULL){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	error=addNewControlToList(temp_control,list);
	if(error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	//label - paint first
	temp_control = new_label(0,0,500,250,"./gfx/textArea.bmp",255,0,255,1,qustion);
	if(temp_control==NULL){
		freeUnconnectedList(list);
		free_control(root->cntrl);
		free(root);
		return -1;
	}
	error=addNewControlToList(temp_control,list);
	if(error<0){
		free_control(root->cntrl);
		free(root);
		return -1;
	}

	set_list_as_children(list,root);
	ui_tree=root;
	error=qustionORtext(flag);
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

	while(!quit)
    {
	while(SDL_PollEvent(&test_event)) {
		if(test_event.type == SDL_QUIT){
			quit=1;
			//freeControlList(ui_tree);
			break;
		}
		switch(test_event.type) {
		 case SDL_MOUSEBUTTONDOWN:
			 find_element_by_coordinates(ui_tree,test_event.button.x,test_event.button.y,&pressed_Button);
			 if(pressed_Button==NULL)
			 {
				 break;
			 }
			 whichGame=pressed_Button->cntrl->buttonChoise;
			 return whichGame;
		}
	}
	}
	return -2;
}

int qustionORtext(qustionWindowsSgin flag){
	linked_list_cntrl list;
	int error;

	list = new_control_list();
	if(list==NULL){
		return -1;
	}
	if(flag==OVERWRITE_SIGN){
		error=newButtonGeneric(list,170,20,YES,emptryButton,1);
		if (error<0){
			return -1;
		}
		error=newButtonGeneric(list,170,190,NO,emptryButton,0);
		if (error<0){
			return -1;
		}
	}
	else if(flag==OK_SIGN){
		error=newButtonGeneric(list,170,190,OK,emptryButton,0);
		if (error<0){
			return -1;
		}
	}
	set_list_as_children(list,ui_tree->children->tail);
	return 0;
}

int handelAI_VS_AI(int *pause){
	int error;
	
	if (cur_game->is_game_over( cur_game->board)==1){
		return 0;
	}
	if ((cur_game)->cur_player==1){
		error=cur_game->handle_computer_move(cur_game->board,cur_game->difficultyP1,cur_game->cur_player);
		if (error<0){
			printf("failed in handle_computer_move\n");
			return -1;
		}
	}
	else {
		error=cur_game->handle_computer_move(cur_game->board,cur_game->difficultyP2,cur_game->cur_player);
		if (error<0){
			printf("failed in handle_computer_move\n");
			return -1;
		}
	}
	(cur_game)->cur_player = (-1)*(cur_game)->cur_player;
	draw_game();

	if (cur_game->is_game_over( cur_game->board)==1){
		if (cur_game->is_victory( cur_game->board) == 1){
			error=cur_game->victoryColor(cur_game->board,1,ui_tree);
			if (error<0){
				printf("failed in victoryColor\n");
				return -1;
			}
		}
		else if (cur_game->is_victory( cur_game->board) == -1){
			error=cur_game->victoryColor(cur_game->board,-1,ui_tree);
			if (error<0){
				printf("failed in victoryColor\n");
				return -1;
			}
		}
		else
		{
			error=newButtonGeneric(ui_tree->children,300,480,"draw",restartGame,0);
			if (error<0){
				printf("failed in makeing draw button\n");
				return -1;
			}
			ui_tree->children->tail->parent=ui_tree;
		}
		draw_ui_tree(ui_tree);
		SDL_Flip(ui_tree->cntrl->srfc);
	}

	*pause=1;
	return 0;
}