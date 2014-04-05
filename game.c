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


int gui_init()
{
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
		printf("ERROR: unable to init SDL: %s\n",SDL_GetError());
		return 0;
	}

	if (TTF_Init() < 0){
		printf("ERROR: unable to init SDL: %s\n",SDL_GetError());
		return 0;
	}

	return 1;
}

int main( int argc, char* args[] )
{
	element_cntrl ui_tree=NULL,pressed_Button=NULL; 
	game *cur_game=NULL;
	SDL_Event test_event; 
	int victory_state = 0;
	int move_success = 0;
	int pause=0;

	gui_init();
	runStartManu(&cur_game,&ui_tree,&pause,&test_event);

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
				pressed_Button->cntrl->pressedButton(&cur_game,&ui_tree,&pause,&test_event);
				break;
			default: //unhandled event
				break;
			}
			if (cur_game!=NULL && cur_game->is_multiplayer==1 && pause==0){
				cur_game->handle_computer_move(cur_game->board,cur_game->difficulty,cur_game->cur_player);
				(cur_game)->cur_player = (-1)*(cur_game)->cur_player;
				(ui_tree)=draw_game(cur_game,ui_tree);
				pause=1;
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

element_cntrl get_default_ui_tree(game *cur_game)
{
	element_cntrl root;
	control* temp_control;
	linked_list_cntrl list; 
	
	root = new_control_element(new_window(0,0,1000,700));
	/*create panel children*/	
	list = new_control_list();

	/*button panel*/
	temp_control = new_panel(700,0,300,1000,255,255,255,1);
	addNewControlToList(temp_control,list);
	set_list_as_children(list,root);

	/*panel children*/
	list = new_control_list();

	/*label - paint first*/
	temp_control = new_label(0,0,0,0,"./gfx/button_label.bmp",255,0,255,1,"this is a text");
	addNewControlToList(temp_control,list);

	/*restart*/
	newButtonGeneric(list,25,20,RESTART,restartGame,0);
	/*save*/
	newButtonGeneric(list,25,120,SAVE,runsaveManu,0);
	/*pause- unpause*/
	newButtonGeneric(list,25,220,PAUSE,setUnpause,0);
	/*main menu*/
	newButtonGeneric(list,25,320,MAIN_MENU,runStartManu,0);
	/*difficulties menu*/
	newButtonGeneric(list,25,420,DIFF,runDiffManu,0);
	/*quit*/
	newButtonGeneric(list,25,520,QUIT,quitGame,0);	
	/* set buttons as panel children*/
	set_list_as_children(list,root->children->head);
	return root;
}

//run window in which a game is chosen
game* runWindow(int mainORLoad,game** prevGame){
	element_cntrl ui_tree,pressed_Button=NULL;
	int whichGame,iterationNum=1;
	SDL_Event test_event; 
	game *newGame=NULL;
	void (*buttonAction)(void* cur_game,void* ui_tree,int *choise,void* test_event)=emptryButton;
	char** captionArray;

	if (mainORLoad==START_SIGN){
		ui_tree=startWindow();
	}
	else {
		captionArray=initialazeChoiseWindow(&buttonAction,&iterationNum,prevGame,mainORLoad);
		ui_tree=choiseWindow(iterationNum,buttonAction,captionArray);
	}
	if (!quit){
		draw_ui_tree(ui_tree);
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
			 if (mainORLoad==SAVE_SIGN || mainORLoad==DIFF_SIGN || mainORLoad==AI_SIGN){
				 pressed_Button->cntrl->pressedButton(prevGame,&ui_tree,&whichGame,&test_event);
				 newGame=*prevGame;
			 }
			 else {
				 pressed_Button->cntrl->pressedButton(&newGame,&ui_tree,&whichGame,&test_event);
			 }
			 freeControlList(ui_tree);
			 return newGame;
			 break;
		 default: //unhandled event
			 break;
		 }
	}
	}
	freeControlList(ui_tree);
	return NULL;
}

//go to main menu,choose game and initiate ui_tree
element_cntrl game_init(game **cur_game,int mainORLoad)
{
	element_cntrl ui_tree;

	*cur_game=runWindow(mainORLoad,cur_game);
	if (*cur_game==NULL){
		return NULL;
	}
	ui_tree = get_default_ui_tree((*cur_game));
	return draw_game(*cur_game,ui_tree);
}

//draw game and update ui_tree by cur_game
element_cntrl draw_game (game *cur_game,element_cntrl prev_ui_tree)
{
	element_cntrl game_panel;

	clear_game_panel(prev_ui_tree);
	game_panel = (cur_game)->panel_function((cur_game)->board,makeMove);
	add_control_element_to_list(prev_ui_tree->children,game_panel);
	game_panel->parent= prev_ui_tree;
	draw_ui_tree(prev_ui_tree);
	SDL_Flip( (prev_ui_tree->cntrl->srfc) );
	return prev_ui_tree;
}

element_cntrl choiseWindow(int iterationNum,void (*buttonAction)(void* cur_game,void* ui_tree,int *choise,void* test_event),char** captionStart){
	element_cntrl root, temp_elem;
	control* temp_control;
	linked_list_cntrl list;
	int i;
	
	root = new_control_element(new_window(0,0,250,500));
	/*create panel children*/	
	list = new_control_list();

	/*button panel*/
	temp_control = new_panel(0,0,250,500,255,255,255,1);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);
	set_list_as_children(list,root);

	/*panel children*/
	list = new_control_list();

	/*label - paint first*/
	temp_control = new_label(0,0,100,80,"./gfx/startPanel.bmp",255,0,255,1,"panel");
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	for (i=1;i<=iterationNum;i++)
	{
		newButtonGeneric(list,20,20+(i-1)*50,captionStart[i-1],buttonAction,i);
	}
	free(captionStart);
	newButtonGeneric(list,20,400,CANCEL,quitGame,0);
	set_list_as_children(list,root->children->head);
	return root;

}

element_cntrl startWindow(){
	element_cntrl root;
	control* temp_control;
	linked_list_cntrl list; 

	root = new_control_element(new_window(0,0,200,400));
	/*create panel children*/	
	list = new_control_list();

	/*button panel*/
	temp_control = new_panel(0,0,200,400,255,255,255,1);
	addNewControlToList(temp_control,list);
	set_list_as_children(list,root);

	/*panel children*/
	list = new_control_list();

	//label - paint first
	//temp_control = new_label(0,0,100,80,"./gfx/button_label.bmp",255,0,255,1,"panel");
	//addNewControlToList(temp_control,list);

	newButtonGeneric(list,20,20,CHOOSE_GAME,goToMainMenu,0);
	newButtonGeneric(list,20,100,LOAD,runLoadManu,0);
	newButtonGeneric(list,20,200,CANCEL,quitGame,0);

	set_list_as_children(list,root->children->head);
	return root;

}

void newButtonGeneric(linked_list_cntrl fathersList,int x,int y,char* caption,void (*pressedButton)(void* cur_game,void* ui_tree,int *quit,void* test_event),int buttonChoise){
	control* temp_control;
	temp_control = new_button(x,y,"./gfx/generic_button.bmp",1,caption,0);
	temp_control->buttonChoise=buttonChoise;
	temp_control->pressedButton=pressedButton;
	addNewControlToList(temp_control,fathersList);
}

void addNewControlToList(control* control,linked_list_cntrl fathersList){
	element_cntrl temp_elem;
	temp_elem = new_control_element(control);
	add_control_element_to_list(fathersList,temp_elem);
}

char** initialazeChoiseWindow(void (**pressedButton)(void* cur_game,void* ui_tree,int *quit,void* test_event),int *iterationNum,game **prevGame,int flag){
	char** captionArray;
	char* buttonName;
	int i;

	if (flag==MAIN_SIGN){
		*pressedButton=chooseGame;
		*iterationNum=3;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		captionArray[0]=(char *)TIC_TAC_TOE_NAME;//casting from const char *
		captionArray[1]=(char *)REVERSI_NAME;
		captionArray[2]=(char *)Connect4_NAME;
	}
	else if (flag==LOAD_SIGN){
		*pressedButton=loadGame;
		*iterationNum=5;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		for (i=0;i<*iterationNum;i++){
			captionArray[i]=(char *)LOAD_SLOT;
			buttonName=(char*)malloc(strlen(captionArray[i])+3);//for the meanwhile a leak;
			sprintf(buttonName,"%s %d",captionArray[i],i);
			//free(captionArray[i]);
			captionArray[i]=buttonName;
		}
	}
	else if (flag==SAVE_SIGN){
		*pressedButton=saveGame;
		*iterationNum=5;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		for (i=0;i<*iterationNum;i++){
			captionArray[i]=(char *)SAVE_SLOT;
			buttonName=(char*)malloc(strlen(captionArray[i])+3);//for the meanwhile a leak;
			sprintf(buttonName,"%s %d",captionArray[i],i);
			//free(captionArray[i]);
			captionArray[i]=buttonName;
		}
	}
	else if (flag==AI_SIGN){
		*pressedButton=setmultiplayer;
		*iterationNum=4;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		captionArray[0]=(char *)AI_1;//casting from const char *
		captionArray[1]=(char *)AI_2;
		captionArray[2]=(char *)AI_3;
		captionArray[3]=(char *)AI_4;
	}
	else {
		if (*prevGame==NULL){
			return NULL;
		}
		*pressedButton=setDifficalty;
		*iterationNum=(*prevGame)->difficulty_num;
		captionArray=(char**)calloc(*iterationNum,sizeof(char*));
		for (i=0;i<*iterationNum;i++){
			captionArray[i]=(char *)DIFFICALTY;
			buttonName=(char*)malloc(strlen(captionArray[i])+3);//for the meanwhile a leak;
			sprintf(buttonName,"%s %d",captionArray[i],i);
			//free(captionArray[i]);
			captionArray[i]=buttonName;
		}
	}
	return captionArray;
}