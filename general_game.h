/*general game structure*/
typedef struct _general_game
{
	/*game info */
	int *board;
	int rows; 
	int cols; 
	int cur_player; 
	int difficulty;

	/*general function*/

	char*		(*get_name)();
	int*		(*get_initial_state)();
	linked_list (*get_state_children)(int* game_state, int player);
	int			(*get_state_score(int* game_state,int player));
	int*		(*get_difficulty_levels());
	element_cntrl (*panel_function(int* game_state));


} general_game;

/*integrate game with load/save*/

/*handle general gui calls : save, difficulty, etc*/

/*manage general gui drawing*/