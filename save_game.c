#include "save_game.h"

void save_game_to_file(char* filename, int **game_state, int player, int cols, int rows)
{
	FILE *file; 
	int i,j=0,first=0;
	
	/* open file for writing */
	file = fopen(filename, "w");
	if (file == NULL)
	{ // complete
		perror("Stand"); 
	}

	/* print user and matrix */
	fprintf(file, "%d\n",player);
	for (i=0; i< rows; i++)
	{
		for(j=0; j <cols; j++)
		{
			if (game_state[i][j] == 0)
			{
				if (!first)
				{
					first=1;
					fprintf(file, "%d", game_state[i][j]);
				}
				else
				{
					fprintf(file, " %d", game_state[i][j]);
				}
			}
		}
		fprintf(file, "\n");
	}
}