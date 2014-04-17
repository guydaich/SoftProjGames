/* general_game.c
*  this serves a a general game class. it is instantiated when a game is chosen	
*  (directly, loaded, etc.), with an appropriarte game. it's members and methods 
*  are updated with game-specific values, and pointers to game-specific function
*  implementations 
*/
#include "general_game.h"
#include "windowsDrawing.h"

extern int gameNum;

extern int quit;
extern game* cur_game;
extern element_cntrl ui_tree;

/* inits a new game */
game* new_game(whichGame game_id)
{
	game *new_game_obj = (game*)malloc(sizeof(game));  //TODO check new_game_obj!=NULL
	gameNum++;
	switch (game_id)
	{
	case TTC:
		new_game_obj->get_name = ttc_get_name;
		new_game_obj->get_state_children = ttc_get_state_children;
		new_game_obj->get_difficulty_levels = ttc_get_difficulty_levels;
		new_game_obj->get_state_score = ttc_get_state_score;
		new_game_obj->panel_function = ttc_panel_function;
		new_game_obj->get_initial_state = ttc_get_initial_state;
		new_game_obj->rows =  TIC_TAC_TOE_ROWS;
		new_game_obj->cols = TIC_TAC_TOE_COLS;
		new_game_obj->cur_player= TTC_PLAYER_1; 
		new_game_obj->handle_mouse_button_down= ttc_handle_mouse_button_down;
		new_game_obj->handle_computer_move=ttc_handle_computer_turn;
		new_game_obj->is_game_over=ttc_is_game_over;
		new_game_obj->is_multiplayer = 0;
		new_game_obj->is_victory = ttc_is_victory;
		new_game_obj->player_has_moves = NULL;
		new_game_obj->difficulty_num=1;
		new_game_obj->victoryColor=color_ttc;
		break;
	case REVERSI:
		new_game_obj->get_name = rv_get_name;
		new_game_obj->get_state_children = rv_get_state_children;
		new_game_obj->get_difficulty_levels = rv_get_difficulty_levels;
		new_game_obj->get_state_score = rv_get_state_score;
		new_game_obj->panel_function = rv_panel_function;
		new_game_obj->get_initial_state = rv_get_initial_state;
		new_game_obj->rows =  REVERSI_ROWS;
		new_game_obj->cols = REVERSI_COLS;
		new_game_obj->cur_player= REVERSI_PLAYER_1;
		new_game_obj->handle_mouse_button_down= rv_handle_mouse_button_down;
		new_game_obj->handle_computer_move=rv_handle_computer_turn;
		new_game_obj->is_game_over=rv_is_game_over;
		new_game_obj->is_multiplayer = 0;
		new_game_obj->is_victory = rv_is_victory;
		new_game_obj->player_has_moves = rv_player_has_moves;
		new_game_obj->difficulty_num=4;
		new_game_obj->victoryColor=color_rv;
		break;
	case CONNECT4:
		new_game_obj->get_name = get_name_C4;
		new_game_obj->get_state_children = get_state_children_C4;
		new_game_obj->get_difficulty_levels = get_difficulty_levels_C4;
		new_game_obj->get_state_score = get_state_score_C4;
		new_game_obj->panel_function = C4_panel_function;
		new_game_obj->get_initial_state = get_initial_state_C4;
		new_game_obj->rows =  CONNECT4_ROWS;
		new_game_obj->cols = CONNECT4_COLS;
		new_game_obj->cur_player= CONNECT4_PLAYER_1;
		new_game_obj->handle_mouse_button_down= C4_handle_mouse_button_down;
		new_game_obj->handle_computer_move=C4_handle_computer_turn;
		new_game_obj->is_game_over=is_game_over_C4;
		new_game_obj->is_multiplayer = 0;
		new_game_obj->is_victory = is_victory_C4;
		new_game_obj->difficulty_num=7;
		new_game_obj->victoryColor=color_c4;
		break;
	default:
		break;
	}

	new_game_obj->board = new_game_obj->get_initial_state();
	new_game_obj->difficultyP1=new_game_obj->get_difficulty_levels()[0];
	new_game_obj->difficultyP2=new_game_obj->get_difficulty_levels()[0];

return new_game_obj;

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
