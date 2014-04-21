
/*check if given player can make a move. if cam returns 1,else 0.*/
int contest_player_has_moves(int* game_state, int player)
{
	int i,j;

	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			if (rv_is_valid_move(game_state, player,i,j))
			{
	            return 1;
            }
		}
	}
	return 0;
}

/* a move is valid iff another piece of the player is encoutered 
   in one or more- this fuction checks if the move is valid*/ 
int contest_is_valid_move (int *game_state, int player, int rows, int cols)
{
	int i=0,t_rows=-1,t_cols=-1, length=0;
	int other = player*(-1); 
	
	//if there is another piece at the same place,can't make this move
	if (game_state[rows*REVERSI_ROWS+cols]!=0)
	{
		return 0;
	}
	// for every possible direction flip enemys pieces
	for (i=0;i<REVERSI_ROWS; i++)
	{
		length = 0; 
		t_rows = rows + move_directions[i][0]; 
		t_cols = cols + move_directions[i][1];
		
		//if the derection lead as out of the board, try another  
		if (t_cols>=REVERSI_COLS || t_rows>=REVERSI_ROWS){
			continue;
		}
		// if encoutered enemy piece at that derection
		while (game_state[t_rows*REVERSI_ROWS + t_cols] == other)
		{
			length++;
			
			t_rows += move_directions[i][0]; 
			t_cols += move_directions[i][1];

			// if encoutered another piece
			if (game_state[t_rows*REVERSI_ROWS + t_cols] == player )
			{
				//	retract moves and change colors
				while (t_rows != rows || t_cols != cols)
				{
					t_rows -= move_directions[i][0]; 
					t_cols -= move_directions[i][1];
					//game_state[t_rows*REVERSI_ROWS + t_cols] = player;
				}
				// finished fliping in this direction, break
				return 1;
			}
		}
	}
	return 0;
}

int contest_num_valid_moves(int* game_state, int player)
{
	int i,j;
	int move_count = 0;

	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			if (rv_is_valid_move(game_state, player,i,j))
			{
	            move_count++;
            }
		}
	}
	return move_count;
}

/*
 * Assuming my_color stores your color and opp_color stores opponent's color
 * '-' indicates an empty square on the board
 * 'b' indicates a black tile and 'w' indicates a white tile on the board
 */
double dynamic_heuristic_evaluation_function(char grid[8][8])  {
        int my_tiles = 0, opp_tiles = 0, i, j, k, my_front_tiles = 0, opp_front_tiles = 0, x, y;
        double p = 0, c = 0, l = 0, m = 0, f = 0, d = 0;
 
        int X1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
        int Y1[] = {0, 1, 1, 1, 0, -1, -1, -1};
 
// Piece difference, frontier disks and disk squares
        for(i=0; i<8; i++)
                for(j=0; j<8; j++)  {
                        if(grid[i][j] == my_color)  {
                                d += V[i][j];
                                my_tiles++;
                        } else if(grid[i][j] == opp_color)  {
                                d -= V[i][j];
                                opp_tiles++;
                        }
                        if(grid[i][j] != '-')   {
                                for(k=0; k<8; k++)  {
                                        x = i + X1[k]; y = j + Y1[k];
                                        if(x >= 0 && x < bound_x && y >= 0 && y < bound_y && grid[x][y] == '-') {
                                                if(grid[i][j] == my_color)  my_front_tiles++;
                                                else opp_front_tiles++;
                                                break;
                                        }
                                }
                        }
                }
        if(my_tiles > opp_tiles)
                p = (100.0 * my_tiles)/(my_tiles + opp_tiles);
        else if(my_tiles < opp_tiles)
                p = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
        else p = 0;
 
        if(my_front_tiles > opp_front_tiles)
                f = -(100.0 * my_front_tiles)/(my_front_tiles + opp_front_tiles);
        else if(my_front_tiles < opp_front_tiles)
                f = (100.0 * opp_front_tiles)/(my_front_tiles + opp_front_tiles);
        else f = 0;
 
// Corner occupancy
        my_tiles = opp_tiles = 0;
        if(grid[0][0] == my_color) my_tiles++;
        else if(grid[0][0] == opp_color) opp_tiles++;
        if(grid[0][7] == my_color) my_tiles++;
        else if(grid[0][7] == opp_color) opp_tiles++;
        if(grid[7][0] == my_color) my_tiles++;
        else if(grid[7][0] == opp_color) opp_tiles++;
        if(grid[7][7] == my_color) my_tiles++;
        else if(grid[7][7] == opp_color) opp_tiles++;
        c = 25 * (my_tiles - opp_tiles);
 
// Corner closeness
        my_tiles = opp_tiles = 0;
        if(grid[0][0] == '-')   {
                if(grid[0][1] == my_color) my_tiles++;
                else if(grid[0][1] == opp_color) opp_tiles++;
                if(grid[1][1] == my_color) my_tiles++;
                else if(grid[1][1] == opp_color) opp_tiles++;
                if(grid[1][0] == my_color) my_tiles++;
                else if(grid[1][0] == opp_color) opp_tiles++;
        }
        if(grid[0][7] == '-')   {
                if(grid[0][6] == my_color) my_tiles++;
                else if(grid[0][6] == opp_color) opp_tiles++;
                if(grid[1][6] == my_color) my_tiles++;
                else if(grid[1][6] == opp_color) opp_tiles++;
                if(grid[1][7] == my_color) my_tiles++;
                else if(grid[1][7] == opp_color) opp_tiles++;
        }
        if(grid[7][0] == '-')   {
                if(grid[7][1] == my_color) my_tiles++;
                else if(grid[7][1] == opp_color) opp_tiles++;
                if(grid[6][1] == my_color) my_tiles++;
                else if(grid[6][1] == opp_color) opp_tiles++;
                if(grid[6][0] == my_color) my_tiles++;
                else if(grid[6][0] == opp_color) opp_tiles++;
        }
        if(grid[7][7] == '-')   {
                if(grid[6][7] == my_color) my_tiles++;
                else if(grid[6][7] == opp_color) opp_tiles++;
                if(grid[6][6] == my_color) my_tiles++;
                else if(grid[6][6] == opp_color) opp_tiles++;
                if(grid[7][6] == my_color) my_tiles++;
                else if(grid[7][6] == opp_color) opp_tiles++;
        }
        l = -12.5 * (my_tiles - opp_tiles);
 
// Mobility
        my_tiles = num_valid_moves(my_color, opp_color, grid);
        opp_tiles = num_valid_moves(opp_color, my_color, grid);
        if(my_tiles > opp_tiles)
                m = (100.0 * my_tiles)/(my_tiles + opp_tiles);
        else if(my_tiles < opp_tiles)
                m = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
        else m = 0;
 
// final weighted score
        double score = (10 * p) + (801.724 * c) + (382.026 * l) + (78.922 * m) + (74.396 * f) + (10 * d);
        return score;
}