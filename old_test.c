void foo()
{
}

/*void print_state_TTC(int* game_state)
{
	int i=0,j=0;

	for (i=0; i < TIC_TAC_TOE_ROWS; i++)
	{
		for(j=0; j < TIC_TAC_TOE_COLS; j++)
		{
			printf("%d",game_state[i*TIC_TAC_TOE_ROWS + j]);
		}
		printf("\n");
	}
}*/

/*
void print_state_RVRS(int* game_state)
{
	int i=0,j=0;

	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			printf("%d\t",game_state[i*REVERSI_ROWS + j]);
		}
		printf("\n");
	}
}


void reversi_test()
{
	int mv_r = 0; 
	int mv_c = 0;
	int comp_move;
	int* board = rv_get_initial_state();
	while(1)
	{
		printf("next move:\n");
		scanf("%d %d",&mv_r,&mv_c);
		rv_make_move(board,mv_r,mv_c,1);
		print_state_RVRS(board);
		if (rv_is_game_over(board))
		{
			break;
		}
		comp_move = get_computer_move(board, 2, rv_get_state_children);
		printf("computer move: %d %d\n",comp_move/REVERSI_ROWS,comp_move%REVERSI_ROWS);
		// problem with minimax - had to cahnge to 1 from -1 in get_computer_move
		rv_make_move(board,comp_move/REVERSI_ROWS,comp_move%REVERSI_ROWS,-1);
		print_state_RVRS(board);
	}
}
*/
/*void ttc_test()
{
	int mv_r = 0; 
	int mv_c = 0;
	int comp_move;
	int* board = get_initial_state();
	while(1)
	{
		printf("next move:\n");
		scanf("%d %d",&mv_r,&mv_c);
		make_move(board,mv_r,mv_c,1);
		print_state(board);
		if (is_game_over(board))
		{

			break;
		}
		comp_move = get_computer_move(board, 9, get_state_children);
		make_move(board,comp_move/TIC_TAC_TOE_ROWS,comp_move%TIC_TAC_TOE_ROWS,-1);
		print_state(board);
	}
}*/

	/*int* game = get_initial_state();*/
	/*int game[] = {1,0,1,0,1,0,1,0,1};
	linked_list list = get_state_children(game,1);
	element elem = list->head;
	int c=0;

	while (elem != NULL)
	{
		c++;
		printf("%d\n",elem->node->score);
		print_state(elem->node->game_state);
		elem = elem->next;
	}

	
	int game[] = 	
		{0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,		
		0, 0, 0, 1, -1, 0, 0, 0,
		0, 0, 0, -1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0};

	linked_list list = rv_get_state_children(game,1);
	element elem = list->head;
	int c=0;

	while (elem != NULL)
	{
		c++;
		printf("%d\n",elem->node->score);
		print_state(elem->node->game_state);
		elem = elem->next;
	}
	*/


