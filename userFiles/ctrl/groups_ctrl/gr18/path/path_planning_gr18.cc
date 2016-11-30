#include "path_planning_gr18.h"
#include "init_pos_gr18.h"
#include "opp_pos_gr18.h"
#include "useful_gr18.h"
#include "path_regulation_gr18.h"
#include "speed_regulation_gr18.h"
#include <math.h>

#define RESOLUTION 2
#define CELL_X 20*RESOLUTION + 2
#define CELL_Y 30*RESOLUTION + 2

#define N_REGISTRED -1
#define OBSTACLE  99
#define GOAL 55

static float tab_cases[CELL_X][CELL_Y];
static int list_cases[100][2] = {{0},{0}};

NAMESPACE_INIT(ctrlGr18);

/*! \brief initialize the path-planning algorithm (memory allocated)
*
* \param[in,out] path path-planning main structure
*/
PathPlanning* init_path_planning(CtrlStruct *cvs)
{
	//variable declaration
	PathPlanning *path;

	// memory allocation
	path = (PathPlanning*)malloc(sizeof(PathPlanning));
	
	// ----- path-planning initialization start ----- //

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

void create_map(void)
{
	// variable initialization
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
		tab_cases[i][0] = OBSTACLE; // Bordure gauche
		tab_cases[i][CELL_Y-1] = OBSTACLE; // Bordure droite
	}
	for (j = 0; j < CELL_Y; j++)
	{
		tab_cases[0][j] = OBSTACLE; //Bordure inférieure
		tab_cases[CELL_X-1][j] = OBSTACLE; //Bordure supérieure
	}
	
	for (j = 1; j <= 5*RESOLUTION; j++)
	{
		for (i = 0; i < RESOLUTION; i++)
		{
			tab_cases[15*RESOLUTION + i][j] = OBSTACLE; // Barrière en bas à gauche (camp jaune départ)
			tab_cases[15*RESOLUTION + i][(25*RESOLUTION + j)] = OBSTACLE; // Barrière en bas à droite (camp bleu départ)
		}
	}

	for (i = 1; i <= 5*RESOLUTION; i++)
	{
		for (j = 0; j < RESOLUTION; j++)
		{
			tab_cases[i][7*RESOLUTION + j] = OBSTACLE; //Barrière en haut à gauche (camp bleu cible)
			tab_cases[i][CELL_Y - 8*RESOLUTION + j] = OBSTACLE; //Barrière en haut à droite (camp jaune cible)
		}
	}

	for (i = 9*RESOLUTION; i <= 12*RESOLUTION; i++)
	{
		for (j = 0; j < RESOLUTION; j++)
		{
			tab_cases[i][12*RESOLUTION + j] = OBSTACLE; //Barrière milieu-gauche
			tab_cases[i][19*RESOLUTION + j] = OBSTACLE; //Barrière milieu-droit
		}
	}
	for (i = 6*RESOLUTION; i <= 9*RESOLUTION; i++)
	{
		for (j = 0; j < 2*RESOLUTION; j++)
		{
			tab_cases[i][15*RESOLUTION + j] = OBSTACLE; // Barrière milieu-milieu verticale
		}
	}

	for (j = 12*RESOLUTION; j <= 19*RESOLUTION; j++)
	{
		for (i = 0; i < RESOLUTION; i++)
		{
			tab_cases[9*RESOLUTION + i][j] = OBSTACLE; // Barrière milieu horizontale
		}
	}
}

void trajectory(CtrlStruct *cvs, double goal_x, double goal_y)
{
	// variables declaration
	RobotPosition *rob_pos;
	
	int i, j;
    int n, k;
    
    int X, Y;
    int goal_X, goal_Y;
    
    float minimum;
    
    // variables initialization
    rob_pos = cvs->rob_pos;
    
    k=0;
    
    X = RESOLUTION*floor(10.0*rob_pos->x) + (CELL_X - 2)/2;
	Y = RESOLUTION*floor(10.0*rob_pos->y) + (CELL_Y - 2)/2;
	goal_X = RESOLUTION*floor(10.0*goal_x) + (CELL_X - 2)/2;
	goal_Y = RESOLUTION*floor(10.0*goal_y) + (CELL_Y - 2)/2;
	
	create_map();
	
    tab_cases[goal_X][goal_Y] = 0;
    list_cases[0][0] = X;
	list_cases[0][1] = Y;

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
	
	minimum = tab_cases[X][Y];
	
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
	/*	
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
	*/
	
	for (i = 0; i <= k; i++)
	{
		tab_cases[list_cases[i][0]][list_cases[i][1]] = GOAL;
	}
	
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (tab_cases[i][j] == OBSTACLE) {
				printf("o ");
			}
			else if (tab_cases[i][j] == GOAL) {
				printf("* ");
			}
			else if (tab_cases[i][j] == N_REGISTRED) {
				printf("+ ");
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}
	
	return;
}

NAMESPACE_CLOSE();
