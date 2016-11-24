#include "path_planning_gr18.h"
#include "init_pos_gr18.h"
#include "opp_pos_gr18.h"
#include "useful_gr18.h"
#include "path_regulation_gr18.h"
#include "speed_regulation_gr18.h"
#include <math.h>

#define CELL_X 22
#define CELL_Y 32

#define N_REGISTRED -1
#define OBSTACLE  99

static float tab_cases[CELL_X][CELL_Y];

NAMESPACE_INIT(ctrlGr18);

/*! \brief initialize the path-planning algorithm (memory allocated)
*
* \param[in,out] path path-planning main structure
*/
PathPlanning* init_path_planning(CtrlStruct *cvs)
{
	//variable declaration
	int i, j;
	
	PathPlanning *path;

	// memory allocation
	path = (PathPlanning*)malloc(sizeof(PathPlanning));
	
	// variable initialization
	
	// ----- path-planning initialization start ----- //

	map_creation(); //création de la map
	
	trajectory(cvs, -0.5, -0.75);

	// ----- path-planning initialization end ----- //
	
	// return structure initialized
	return path;
}

/*! \brief close the path-planning algorithm (memory released)
*
* \param[in,out] path path-planning main structure
*/
void free_path_planning(PathPlanning *path)
{
	// ----- path-planning memory release start ----- //


	// ----- path-planning memory release end ----- //

	free(path);
}

void map_creation(void)
{
	int i, j;

	for (i = 0; i < CELL_X; i++)
	{
		for (j = 0; j < CELL_Y; j++)
		{
			tab_cases[i][j] = N_REGISTRED;
		}
	}

	for (i = 0; i < CELL_X; i++)
	{
		tab_cases[i][0] = OBSTACLE; // Bordure supérieure
		tab_cases[i][CELL_Y-1] = OBSTACLE; // Bordure inférieure
	}
	for (j = 0; j < CELL_Y; j++)
	{
		tab_cases[0][j] = OBSTACLE; //Bordure gauche
		tab_cases[CELL_X-1][j] = OBSTACLE; //Bordure droite
	}
	
	for (j = 1; j <= 5; j++)
	{
		tab_cases[16][j] = OBSTACLE; // Barrière en bas à gauche (camp jaune départ)
		tab_cases[16][25 + j] = OBSTACLE; // Barrière en bas à droite (camp bleu départ)
	}

	for (i = 1; i <= 5; i++)
	{
		tab_cases[i][7] = OBSTACLE; //Barrière en haut à gauche (camp bleu cible)
		tab_cases[i][24] = OBSTACLE; //Barrière en haut à droite (camp jaune cible)
	}

	for (i = 10; i <= 13; i++)
	{
		tab_cases[i][12] = OBSTACLE; //Barrière milieu-gauche
		tab_cases[i][19] = OBSTACLE; //Barrière milieu-droit
	}
	for (i = 6; i <= 9; i++)
	{
		tab_cases[i][15] = OBSTACLE; // Barrière milieu-milieu verticale
		tab_cases[i][16] = OBSTACLE;
	}

	for (j = 12; j <= 18; j++)
	{
		tab_cases[10][j] = OBSTACLE; // Barrière milieu horizontale
	}
	
	return;
}

void number_assigment(int X, int Y)
{
    int i, j;
    int k, l;
    int n;

    tab_cases[X][Y] = 0;

    for(n = X; n > 0; n--)
    {
        for (i = 0; i < CELL_X; i++)
        {
            for (j = 0; j < CELL_Y; j++)
            {
                if (tab_cases[i][j] != N_REGISTRED && tab_cases[i][j] != OBSTACLE)
                {
                    if (tab_cases[i-1][j] == N_REGISTRED)
                    {
                        tab_cases[i-1][j] = tab_cases[i][j] + 1;
                    }
                    if (tab_cases[i+1][j] == N_REGISTRED)
                    {
                        tab_cases[i+1][j] = tab_cases[i][j] + 1;
                    }
                    if (tab_cases[i][j-1] == N_REGISTRED)
                    {
                        tab_cases[i][j-1] = tab_cases[i][j] + 1;
                    }
                    if (tab_cases[i][j+1] == N_REGISTRED)
                    {
                        tab_cases[i][j+1] = tab_cases[i][j] + 1;
                    }
                    if (tab_cases[i-1][j-1] == N_REGISTRED)
                    {
                        tab_cases[i-1][j-1] = tab_cases[i][j] + sqrt(2);
                    }
                    if (tab_cases[i-1][j+1] == N_REGISTRED)
                    {
                        tab_cases[i-1][j+1] = tab_cases[i][j] + sqrt(2);
                    }
                    if (tab_cases[i+1][j-1] == N_REGISTRED)
                    {
                        tab_cases[i+1][j-1] = tab_cases[i][j] + sqrt(2);
                    }
                    if (tab_cases[i+1][j+1] == N_REGISTRED)
                    {
                        tab_cases[i+1][j+1] = tab_cases[i][j] + sqrt(2);
                    }
                }
            }
        }
    }

    return;
}

void trajectory(CtrlStruct *cvs, double goal_x, double goal_y)
{
	int goal_X = floor(10.0*goal_x) + (CELL_X - 2)/2;
	int goal_Y = floor(10.0*goal_y) + (CELL_Y - 2)/2;
	
	number_assigment(goal_X, goal_Y);
	
	int X = 19;
	int Y = 27;
	
	int i, j;
	int list_cases[100][2] = {{0},{0}};
	list_cases[0][0] = X;
	list_cases[0][1] = Y;
	float minimum = tab_cases[X][Y];
	
	int k=0;
	
	while ((list_cases[k][0] != goal_X) || (list_cases[k][1] != goal_Y))
	{
		k++;
		for (i = -1; i <= 1; i++)
		{
			for (j = -1; j <= 1; j++)
			{
				if (tab_cases[list_cases[k-1][0]+i][list_cases[k-1][1]+j] < minimum)
				{
					minimum = tab_cases[list_cases[k-1][0]+i][list_cases[k-1][1]+j];
					list_cases[k][0] = list_cases[k-1][0]+i;
					list_cases[k][1] = list_cases[k-1][1]+j;
				}
			}
		}
	}
	
	
	for (i = 0; i <= k; i++)
	{
		tab_cases[list_cases[i][0]][list_cases[i][1]] = 55.0;
	}
	
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (tab_cases[i][j] < 10) {
				printf("%1.2f ", tab_cases[i][j]);
			}
			else {
				printf("%1.1f ", tab_cases[i][j]);
			}
		}
		printf("\n");
	}
	
	return;
}

NAMESPACE_CLOSE();
