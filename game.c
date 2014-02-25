#include "game.h"
int quit=0;
extern char* TIC_TAC_TOE_NAME;
char* RESTART="restart";
char* QUIT="quit";
char* MAIN_MENU="mainMenu";

int gui_init()
{
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
		printf("ERROR: unable to init SDL: %s\n",SDL_GetError());
		return 0;
	}

	return 1;
}

int main( int argc, char* args[] )
{
	element_cntrl ui_tree=NULL,pressed_Button=NULL; 
	game *cur_game;
	SDL_Event test_event; 

	gui_init();
	ui_tree=game_init(&cur_game);

	while(!quit)
    {
	while(SDL_PollEvent(&test_event)) {
		if(test_event.type == SDL_QUIT){
			quit=1;
			break;
		}
		 switch(test_event.type) {
		 case SDL_MOUSEBUTTONDOWN:
			 if (test_event.button.x<=600 && test_event.button.y<=600)
			 {
				ttc_handle_mouse_button_down(&test_event, ui_tree,cur_game->board);
				ui_tree=draw_game(cur_game,ui_tree);
			 }
			 else {
				 find_element_by_coordinates(ui_tree,test_event.button.x,test_event.button.y,&pressed_Button);
				 if (strcmp(pressed_Button->cntrl->caption,RESTART)==0){
					cur_game->board=ttc_get_initial_state();
					ui_tree=draw_game(cur_game,ui_tree);
					break;
				 }
				 if (strcmp(pressed_Button->cntrl->caption,QUIT)==0){
					quit=1;
					break;
				 }
				 if (strcmp(pressed_Button->cntrl->caption,MAIN_MENU)==0){
					ui_tree=game_init(&cur_game);
					break;
				 }
			 }
			 break;
		 default: //unhandled event
			 printf("hi");
			 break;
		 }
	}
	}

	return 0;
}

element_cntrl get_default_ui_tree(game *cur_game)
{
	element_cntrl root, temp_elem;
	control* temp_control;
	linked_list_cntrl list; 
	int* diff;
	int i;
	
	root = new_control_element(new_window(0,0,1000,700));
	/*create panel children*/	
	list = new_control_list();

	/*button panel*/
	temp_control = new_panel(600,0,400,1000,255,255,255);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);
	set_list_as_children(list,root);


	/*panel children*/
	list = new_control_list();

	/*label - paint first*/
	temp_control = new_label(650,0,330,80,"./gfx/button_label.bmp",255,0,255,1);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*restart*/
	temp_control = new_button(675,100,200,65,"./gfx/btn_restart.bmp",255,0,255,1,RESTART);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*save*/
	temp_control = new_button(675,200,330,80,"./gfx/btn_save.bmp",255,0,255,1,NULL);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);


	/*pause- unpause*/
	temp_control = new_button(675,300,330,80,"./gfx/btn_unpause.bmp",255,0,255,1,NULL);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*main menu*/
	temp_control = new_button(675,400,330,80,"./gfx/btn_menu.bmp",255,0,255,1,MAIN_MENU);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*difficulties*/
	diff = cur_game->get_difficulty_levels();
	for (i=0; i< 1; i++)
	{
		temp_control = new_button(675,(i+5)*100,330,80,"./gfx/btn_diff_1.bmp",255,0,255,1,NULL);
		temp_elem = new_control_element(temp_control);
		add_control_element_to_list(list,temp_elem);
	}

	/*quit*/
	temp_control = new_button(675,(i+5)*100+10,330,80,"./gfx/btn_quit.bmp",255,0,255,1,QUIT);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);
	
	/* set buttons as panel children*/
	set_list_as_children(list,root->children->head);
	return root;
}

element_cntrl mainMenuWindow(){
	element_cntrl root, temp_elem;
	control* temp_control;
	linked_list_cntrl list; 
	
	root = new_control_element(new_window(0,0,150,200));
	/*create panel children*/	
	list = new_control_list();

	/*button panel*/
	temp_control = new_panel(0,0,200,200,255,255,255);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);
	set_list_as_children(list,root);

	/*panel children*/
	list = new_control_list();

	/*label - paint first*/
	temp_control = new_label(0,0,100,80,"./gfx/startPanel.bmp",255,0,255,1);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*restart*/
	temp_control = new_button(20,20,100,60,"./gfx/TTC.bmp",255,0,255,1,TIC_TAC_TOE_NAME);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	set_list_as_children(list,root->children->head);
	return root;

}

//run window in which a game is chosen
game* runMainMenu(){
	element_cntrl ui_tree,pressed_Button=NULL;
	SDL_Event test_event; 

	ui_tree =mainMenuWindow();
	draw_ui_tree(ui_tree);
    SDL_Flip( ui_tree->cntrl->srfc );

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
			 if (strcmp(pressed_Button->cntrl->caption,TIC_TAC_TOE_NAME)==0){
				 return new_game(TTC);
			 }
			 break;
		 default: //unhandled event
			 printf("hi");
			 break;
		 }
	}
	}

	return NULL;
}

//go to main menu,choose game and initiate ui_tree
element_cntrl game_init(game **cur_game)
{
	element_cntrl ui_tree;

	*cur_game=runMainMenu();
	(*cur_game)->board = (*cur_game)->get_initial_state();
	ui_tree = get_default_ui_tree((*cur_game));
	return draw_game(*cur_game,ui_tree);
}

//draw game and update ui_tree by cur_game
element_cntrl draw_game (game *cur_game,element_cntrl prev_ui_tree)
{
	element_cntrl game_panel;

	clear_game_panel(prev_ui_tree);
	game_panel = (cur_game)->panel_function((cur_game)->board);
	add_control_element_to_list(prev_ui_tree->children,game_panel);
	game_panel->parent= prev_ui_tree;
	draw_ui_tree(prev_ui_tree);
	SDL_Flip( (prev_ui_tree->cntrl->srfc) );
	return prev_ui_tree;
}


//TODO: MUST clear prev game panel before adding a new one- the the reason for "memory" in restart
