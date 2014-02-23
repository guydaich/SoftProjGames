#include "game.h"

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
	
	control *window;
	element_cntrl ui_tree,game_panel; 
	game *cur_game = new_game(TTC);

	int* game = cur_game->get_initial_state();

	SDL_Event test_event; 
	int quit=0;

	gui_init();
	ui_tree = get_default_ui_tree(cur_game);
	game_panel = cur_game->panel_function(game);
	add_control_element_to_list(ui_tree->children,game_panel);
	game_panel->parent= ui_tree;
	
	draw_ui_tree(ui_tree);
    SDL_Flip( ui_tree->cntrl->srfc );

	while(!quit)
    {
	while(SDL_PollEvent(&test_event)) {
		 switch(test_event.type) {
		 case SDL_MOUSEBUTTONDOWN:
			 ttc_handle_mouse_button_down(&test_event, ui_tree,game);
			 game_panel = cur_game->panel_function(game);
			 add_control_element_to_list(ui_tree->children,game_panel);
			 game_panel->parent= ui_tree;
			 draw_ui_tree(ui_tree);
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
	
	root = new_control_element(new_window(0,0,1000,1000));
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
	temp_control = new_button(650,0,330,80,"./gfx/button_label.bmp",255,0,255,1);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*restart*/
	temp_control = new_button(675,100,200,65,"./gfx/btn_restart.bmp",255,0,255,1);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*save*/
	temp_control = new_button(675,200,330,80,"./gfx/btn_save.bmp",255,0,255,1);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);


	/*pause- unpause*/
	temp_control = new_button(675,300,330,80,"./gfx/btn_unpause.bmp",255,0,255,1);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*main menu*/
	temp_control = new_button(675,400,330,80,"./gfx/btn_menu.bmp",255,0,255,1);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);

	/*difficulties*/
	diff = cur_game->get_difficulty_levels();
	for (i=0; i< 2; i++)
	{
		temp_control = new_button(675,(i+5)*100,330,80,"./gfx/btn_diff_1.bmp",255,0,255,1);
		temp_elem = new_control_element(temp_control);
		add_control_element_to_list(list,temp_elem);
	}

	/*quit*/
	temp_control = new_button(675,(i+6)*100,330,80,"./gfx/btn_quit.bmp",0,0,0,0);
	temp_elem = new_control_element(temp_control);
	add_control_element_to_list(list,temp_elem);
	
	/* set buttons as panel children*/
	set_list_as_children(list,root->children->head);
	return root;
}

