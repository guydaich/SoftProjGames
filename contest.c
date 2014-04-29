/* improved scoring function, uses a combination of heuristics, based on the following article :
 * http://courses.cs.washington.edu/courses/cse573/04au/Project/mini1/RUSSIA/Final_Paper.pdf */

#define REVERSI_ROWS 8
#define REVERSI_COLS 8

int contest_player_has_moves(int* game_state, int player);
int contest_is_valid_move (int *game_state, int player, int rows, int cols);
int contest_num_valid_moves(int* game_state, int player);
int calc_score_reversi(int* board);

int move_directions[8][2] = {{1,1},{-1,-1},{1,-1},{-1,1},{0,1},{1,0},{-1,0},{0,-1}};

/*check if given player can make a move. if cam returns 1,else 0.*/
int contest_player_has_moves(int* game_state, int player)
{
	int i,j;

	for (i=0; i < REVERSI_ROWS; i++)
	{
		for(j=0; j < REVERSI_COLS; j++)
		{
			if (contest_is_valid_move(game_state, player,i,j))
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
		if (t_cols>=REVERSI_COLS || t_rows>=REVERSI_ROWS|| t_cols<0 || t_rows<0){
			continue;
		}
		// if encoutered enemy piece at that derection
		while (game_state[t_rows*REVERSI_ROWS + t_cols] == other)
		{

			if (t_cols>=REVERSI_COLS || t_rows>=REVERSI_ROWS|| t_cols<0 || t_rows<0){
				break;
			}
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
			if (contest_is_valid_move(game_state, player,i,j))
			{
	            move_count++;
            }
		}
	}
	return move_count;
}

/*
 * Assuming my_color stores your color and opp_color stores opponent's color
 * 0 indicates an empty square on the board
 * '1' indicates a black tile and '-1' indicates a white tile on the board
 */
int calc_score_reversi(int* board)  {
        int P1_pieces = 0, P2_pieces = 0, i, j, k, P1_tiles = 0,P2_tiles = 0, x, y,my_color=1;
        int p = 0, c = 0, l = 0, m = 0, f = 0, d = 0,opp_color=-1,bound_x=8,bound_y=8;
 
        int line_wight[] = {-1, -1, 0, 1, 1, 1, 0, -1};
        int col_wight[] = {0, 1, 1, 1, 0, -1, -1, -1};
        int V[8][8]={   {4,-3,2,2,2,2,-3,4},
    					{-3,-4,-1,-1,-1,-1,-4,-3},
    					{2,-1,1,0,0,1,-1,2},
    					{2,-1,0,1,1,0,-1,2},
    					{2,-1,0,1,1,0,-1,2},
    					{2,-1,1,0,0,1,-1,2},
    					{-3,-4,-1,-1,-1,-1,-4,-3},
    					{4,-3,2,2,2,2,-3,4}
    				};
 
        for(i=0; i<8; i++)
                for(j=0; j<8; j++)  {
                        if(board[i*8+j] == my_color)  {
                                d += V[i][j];
                                P1_pieces++;
                        } else if(board[i*8+j] == opp_color)  {
                                d -= V[i][j];
                                P2_pieces++;
                        }
                        if(board[i*8+j] != 0)   {
                                for(k=0; k<8; k++)  {
                                        x = i + line_wight[k]; y = j + col_wight[k];
                                        if(x >= 0 && x < bound_x && y >= 0 && y < bound_y && board[x*8+y] == 0) {
                                                if(board[i*8+j] == my_color)  P1_tiles++;
                                                else P2_tiles++;
                                                break;
                                        }
                                }
                        }
                }
        if(P1_pieces > P2_pieces)
                p = (100.0 * P1_pieces)/(P1_pieces + P2_pieces);
        else if(P1_pieces < P2_pieces)
                p = -(100.0 * P2_pieces)/(P1_pieces + P2_pieces);
        else p = 0;
 
        if(P1_tiles >P2_tiles)
                f = -(100.0 * P1_tiles)/(P1_tiles +P2_tiles);
        else if(P1_tiles <P2_tiles)
                f = (100.0 *P2_tiles)/(P1_tiles +P2_tiles);
        else f = 0;
 
        P1_pieces = P2_pieces = 0;
        if(board[0] == my_color) P1_pieces++;
        else if(board[0] == opp_color) P2_pieces++;
        if(board[7] == my_color) P1_pieces++;
        else if(board[7] == opp_color) P2_pieces++;
        if(board[7*8] == my_color) P1_pieces++;
        else if(board[7*8] == opp_color) P2_pieces++;
        if(board[7*8+7] == my_color) P1_pieces++;
        else if(board[7*8+7] == opp_color) P2_pieces++;
        c = 25 * (P1_pieces - P2_pieces);

        P1_pieces = P2_pieces = 0;
        if(board[0] == 0)   {
                if(board[1] == my_color) P1_pieces++;
                else if(board[1] == opp_color) P2_pieces++;
                if(board[9] == my_color) P1_pieces++;
                else if(board[9] == opp_color) P2_pieces++;
                if(board[8] == my_color) P1_pieces++;
                else if(board[8] == opp_color) P2_pieces++;
        }
        if(board[7] == 0)   {
                if(board[6] == my_color) P1_pieces++;
                else if(board[6] == opp_color) P2_pieces++;
                if(board[14] == my_color) P1_pieces++;
                else if(board[14] == opp_color) P2_pieces++;
                if(board[15] == my_color) P1_pieces++;
                else if(board[15] == opp_color) P2_pieces++;
        }
        if(board[7*8] == 0)   {
                if(board[7*8+1] == my_color) P1_pieces++;
                else if(board[7*8+1] == opp_color) P2_pieces++;
                if(board[6*8+1] == my_color) P1_pieces++;
                else if(board[6*8+1] == opp_color) P2_pieces++;
                if(board[6*8] == my_color) P1_pieces++;
                else if(board[6*8] == opp_color) P2_pieces++;
        }
        if(board[7*8+7] == 0)   {
                if(board[6*8+7] == my_color) P1_pieces++;
                else if(board[6*8+7] == opp_color) P2_pieces++;
                if(board[6*8+6] == my_color) P1_pieces++;
                else if(board[6*8+6] == opp_color) P2_pieces++;
                if(board[7*8+6] == my_color) P1_pieces++;
                else if(board[7*8+6] == opp_color) P2_pieces++;
        }
        l = -12 * (P1_pieces - P2_pieces);
 
        P1_pieces = contest_num_valid_moves(board,my_color);
        P2_pieces = contest_num_valid_moves(board,opp_color);
        if(P1_pieces > P2_pieces)
                m = (100 * P1_pieces)/(P1_pieces + P2_pieces);
        else if(P1_pieces < P2_pieces)
                m = -(100 * P2_pieces)/(P1_pieces + P2_pieces);
        else m = 0;
 
        int score = (10 * p) + (801 * c) + (382* l) + (78 * m) + (74* f) + (10 * d);
        return score;
}