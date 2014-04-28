#include "game.h"

int quit=0;
extern int gameNum;

extern game* cur_game;
extern element_cntrl ui_tree;


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
	error=go_to_start_menu(&pause,&test_event);
	if (error<0 || cur_game==NULL){
		if (quit==1){
			free_control_list(ui_tree);
			if (cur_game!=NULL)
			{
				free(cur_game->board);
				free(cur_game);
			}
		}
		TTF_Quit();
		SDL_Quit();
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
				error=pressed_Button->cntrl->pressed_button(&pause,&test_event);
				if (error<0 ||cur_game==NULL){
					printf("a catchAble error occured\n");
					TTF_Quit();
					SDL_Quit();
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
					free_control_list(ui_tree);
					if (cur_game!=NULL)
					{
						free(cur_game->board);
						free(cur_game);
				}
				TTF_Quit();
				SDL_Quit();
				return 0;
				}
			}
		}
	}
	free_control_list(ui_tree);
	if (cur_game!=NULL)
	{
		free(cur_game->board);
		free(cur_game);
	}
	TTF_Quit();
	SDL_Quit();
	return 0;
}
