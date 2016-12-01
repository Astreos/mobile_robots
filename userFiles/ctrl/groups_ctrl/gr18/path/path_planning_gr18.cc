#include "path_planning_gr18.h"
#include "init_pos_gr18.h"
#include "opp_pos_gr18.h"
#include "useful_gr18.h"
#include "path_regulation_gr18.h"
#include "speed_regulation_gr18.h"
#include <math.h>

#define RESOLUTION 2
#define CELL_X (20*RESOLUTION + 2)
#define CELL_Y (30*RESOLUTION + 2)

#define N_REGISTRED -1
#define OBSTACLE 99
#define GOAL 55

NAMESPACE_INIT(ctrlGr18);

/*! \brief initialize the path-planning algorithm (memory allocated)
*
* \param[in,out] path path-planning main structure
*/
PathPlanning* init_path_planning()
{
	//variable declaration
	PathPlanning *path;
	int i, j;
	
	// memory allocation
	path = (PathPlanning*) malloc(sizeof(PathPlanning));
	if (path == NULL) {exit(0);}
	
	// ----- path-planning initialization start ----- //
	
	// map
	path->map = (float**) malloc(CELL_X*sizeof(float*));
	if (path->map == NULL) {exit(0);}
	
	for(i=0; i<CELL_X; i++)
	{
		path->map[i] = (float*) malloc(CELL_Y*sizeof(float));
		if (path->map[i] == NULL) {exit(0);}
		
		for(j=0; j<CELL_Y; j++)
		{
			path->map[i][j] = N_REGISTRED;
		}
	}
	
	// list_goal
	path->list_goal = (int**) malloc(10*RESOLUTION*sizeof(int*));
	if (path->list_goal == NULL) {exit(0);}

	for(i=0; i<10*RESOLUTION; i++)
	{
		path->list_goal[i] = (int*) malloc(2*sizeof(int));
		if (path->list_goal[i] == NULL) {exit(0);}

		for(j=0; j<2; j++)
		{
			path->list_goal[i][j] = 0;
		}
	}
	
	// rob_pos_XY
	path->rob_pos_XY = (PositionXY*) malloc(sizeof(PositionXY*));
	if (path->rob_pos_XY == NULL) {exit(0);}
	
	// goal_XY
	path->goal_XY = (PositionXY*) malloc(sizeof(PositionXY*));
	if (path->rob_pos_XY == NULL) {exit(0);}
	
	// flag
	path->flag = 0;

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
	// variable declaration
	int i;
	
	// ----- path-planning memory release start ----- //
	
	// map
	for(i=0; i<CELL_X; i++)
	{
		free(path->map[i]);
	}
	free(path->map);
	
	// list_goal
	for(i=0; i<100; i++)
	{
		free(path->list_goal[i]);
	}
	free(path->list_goal);

	// ----- path-planning memory release end ----- //

	free(path);
	
	return;
}

void trajectory(CtrlStruct *cvs, double goal_x, double goal_y)
{
	// variables declaration
	RobotPosition *rob_pos;
	PathPlanning *path;
	
	int i, j;
    
    // variables initialization
    rob_pos = cvs->rob_pos;
    path = cvs->path;
    
    path->rob_pos_XY->X = x_to_X(rob_pos->x);
	path->rob_pos_XY->Y = y_to_Y(rob_pos->y);
	path->goal_XY->X = x_to_X(goal_x);
	path->goal_XY->Y = y_to_Y(goal_y);
	
	create_map(cvs);
	
	assign_numbers(cvs);
	
	find_path(cvs);
	
	/*
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (path->map[i][j] < 10) {
				printf("%1.2f ", path->map[i][j]);
			}
			else {
				printf("%1.1f ", path->map[i][j]);
			}
		}
		printf("\n");
	}
	*/
	
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (path->map[i][j] == OBSTACLE) {
				printf("o");
			}
			else if (path->map[i][j] == GOAL) {
				printf("*");
			}
			else if (path->map[i][j] == N_REGISTRED) {
				printf("+");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
	
	path->flag = 1;
	
	return;
}

void create_map(CtrlStruct *cvs)
{
	// variables declaration
	PathPlanning *path;
	int i, j;
	
	// variables initialization
	path = cvs->path;
	
	for (i = 0; i < CELL_X; i++)
	{
		for (j = 0; j < CELL_Y; j++)
		{
			path->map[i][j] = N_REGISTRED;
		}
	}

	for (i = 0; i < CELL_X; i++)
	{
		path->map[i][0] = OBSTACLE; // Bordure gauche
		path->map[i][CELL_Y-1] = OBSTACLE; // Bordure droite
	}
	for (j = 0; j < CELL_Y; j++)
	{
		path->map[0][j] = OBSTACLE; //Bordure inf�rieure
		path->map[CELL_X-1][j] = OBSTACLE; //Bordure sup�rieure
	}
	
	for (j = 1; j <= 5*RESOLUTION; j++)
	{
		for (i = 0; i < RESOLUTION; i++)
		{
			path->map[15*RESOLUTION + i][j] = OBSTACLE; // Barri�re en bas � gauche (camp jaune d�part)
			path->map[15*RESOLUTION + i][(25*RESOLUTION + j)] = OBSTACLE; // Barri�re en bas � droite (camp bleu d�part)
		}
	}

	for (i = 1; i <= 5*RESOLUTION; i++)
	{
		for (j = 0; j < RESOLUTION; j++)
		{
			path->map[i][7*RESOLUTION + j] = OBSTACLE; //Barri�re en haut � gauche (camp bleu cible)
			path->map[i][CELL_Y - 8*RESOLUTION + j] = OBSTACLE; //Barri�re en haut � droite (camp jaune cible)
		}
	}

	for (i = 9*RESOLUTION; i <= 12*RESOLUTION; i++)
	{
		for (j = 0; j < RESOLUTION; j++)
		{
			path->map[i][12*RESOLUTION + j] = OBSTACLE; //Barri�re milieu-gauche
			path->map[i][19*RESOLUTION + j] = OBSTACLE; //Barri�re milieu-droit
		}
	}
	for (i = 6*RESOLUTION; i <= 9*RESOLUTION; i++)
	{
		for (j = 0; j < 2*RESOLUTION; j++)
		{
			path->map[i][15*RESOLUTION + j] = OBSTACLE; // Barri�re milieu-milieu verticale
		}
	}

	for (j = 12*RESOLUTION; j <= 19*RESOLUTION; j++)
	{
		for (i = 0; i < RESOLUTION; i++)
		{
			path->map[9*RESOLUTION + i][j] = OBSTACLE; // Barri�re milieu horizontale
		}
	}
	
	return;
}

void assign_numbers(CtrlStruct *cvs)
{
	// variable declaration
	PathPlanning *path;
	int i, j, n;
	
	// variables initialization
	path = cvs->path;
	
	path->map[path->goal_XY->X][path->goal_XY->Y] = 0;
    path->list_goal[0][0] = path->rob_pos_XY->X;
	path->list_goal[0][1] = path->rob_pos_XY->Y;

    for(n = path->rob_pos_XY->X; n > 0; n--)
    {
        for (i = 0; i < CELL_X; i++)
        {
            for (j = 0; j < CELL_Y; j++)
            {
                if (path->map[i][j] != N_REGISTRED && path->map[i][j] != OBSTACLE)
                {
                    if (path->map[i-1][j] == N_REGISTRED)
                    {
                        path->map[i-1][j] = path->map[i][j] + 1;
                    }
                    if (path->map[i+1][j] == N_REGISTRED)
                    {
                        path->map[i+1][j] = path->map[i][j] + 1;
                    }
                    if (path->map[i][j-1] == N_REGISTRED)
                    {
                        path->map[i][j-1] = path->map[i][j] + 1;
                    }
                    if (path->map[i][j+1] == N_REGISTRED)
                    {
                        path->map[i][j+1] = path->map[i][j] + 1;
                    }
                    if (path->map[i-1][j-1] == N_REGISTRED)
                    {
                        path->map[i-1][j-1] = path->map[i][j] + sqrt(2);
                    }
                    if (path->map[i-1][j+1] == N_REGISTRED)
                    {
                        path->map[i-1][j+1] = path->map[i][j] + sqrt(2);
                    }
                    if (path->map[i+1][j-1] == N_REGISTRED)
                    {
                        path->map[i+1][j-1] = path->map[i][j] + sqrt(2);
                    }
                    if (path->map[i+1][j+1] == N_REGISTRED)
                    {
                        path->map[i+1][j+1] = path->map[i][j] + sqrt(2);
                    }
                }
            }
        }
    }
    
    return;
}

void find_path(CtrlStruct *cvs)
{
	// variable declaration
	PathPlanning *path;
	int i, j, k;
	float minimum;
	
	// variables initialization
	path = cvs->path;
	
	minimum = path->map[path->rob_pos_XY->X][path->rob_pos_XY->Y];
	k=0;
	
	while ((path->list_goal[k][0] != path->goal_XY->X) || (path->list_goal[k][1] != path->goal_XY->Y))
	{
		k++;
		for (i = -1; i <= 1; i++)
		{
			for (j = -1; j <= 1; j++)
			{
				if (path->map[path->list_goal[k-1][0]+i][path->list_goal[k-1][1]+j] < minimum)
				{
					minimum = path->map[path->list_goal[k-1][0]+i][path->list_goal[k-1][1]+j];
					
					if (k > 9)
					{
						int** list_goal_realloc;
						
						path->list_goal = (int**) realloc(path->list_goal, 10*sizeof(int*) + (k-9)*(sizeof(int*)));
						if (path->list_goal == NULL) {exit(0);}
						
						path->list_goal[k] = (int*) malloc(2*sizeof(int));
						if (path->list_goal[k] == NULL) {exit(0);}
					}
					
					path->list_goal[k][0] = path->list_goal[k-1][0]+i;
					path->list_goal[k][1] = path->list_goal[k-1][1]+j;
				}
			}
		}
	}
	
	path->nb_goals = k;
	
	for (i = 0; i <= k; i++)
	{
		path->map[path->list_goal[i][0]][path->list_goal[i][1]] = GOAL;
	}
	
	return;
}

int x_to_X(double x)
{
	return RESOLUTION*floor(10.0*x) + (CELL_X - 2)/2;
}

int y_to_Y(double y)
{
	return RESOLUTION*floor(10.0*y) + (CELL_Y - 2)/2;
}

double X_to_x(int X)
{
	return ((X - CELL_X - 2.0)/RESOLUTION -0.5)/10;
}

double Y_to_y(int Y)
{
	return ((Y - CELL_Y - 2.0)/RESOLUTION -0.5)/10;
}

NAMESPACE_CLOSE();
