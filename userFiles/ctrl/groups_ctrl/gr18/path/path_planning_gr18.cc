#include "path_planning_gr18.h"
#include "init_pos_gr18.h"
#include "opp_pos_gr18.h"
#include "useful_gr18.h"
#include "path_regulation_gr18.h"
#include "speed_regulation_gr18.h"
#include <math.h>
#include <iostream>

enum {RUN, TURN, CONTINUE};

#define WIDTH 2
#define HEIGHT 3

#define CELL_X 22
#define CELL_Y 32

#define N_REGISTRED -1
#define OBSTACLE  99

#define SENS_BAS 0
#define SENS_HAUT 1
#define SENS_GAUCHE 2
#define SENS_DROITE 3

static int sens = SENS_BAS;
static float tab_cases[CELL_X][CELL_Y];
static int goal_flag = 0;
static int flag = TURN;
static int direction_pre = 0;


NAMESPACE_INIT(ctrlGr18);

/*! \brief initialize the path-planning algorithm (memory allocated)
*
* \param[in,out] path path-planning main structure
*/
PathPlanning* init_path_planning(CtrlStruct *cvs, RobotPosition *robposition)
{
	PathPlanning *path;

	// memory allocation
	path = (PathPlanning*)malloc(sizeof(PathPlanning));
	
	// variable initialization
	path->goal_id = 0;
	int x = 11, y = 15;
	int i, j;

	// ----- path-planning initialization start ----- //

	mapcreating(); //création de la map
	
	number_assigment(19,7);

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

/*
	path->rob_pos_XY = (RobotPosition*)malloc(sizeof(RobotPosition));
	path->rob_goal_XY = (RobotPosition*)malloc(sizeof(RobotPosition));

	path->rob_pos_XY->x = 17;
	path->rob_pos_XY->y = 27;

	// ----- path-planning initialization end ----- //
*/
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

void mapcreating()
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

void number_assigment(int x, int y)
{
    int i, j;
    int k, l;
    int nb_cases = 559; // 22 * 32 - 144 obstacles - la case du goal = 559

    tab_cases[x][y] = 0;

    for(nb_cases = 559; nb_cases > 0; nb_cases--)
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
                        nb_cases--;
                    }
                    if (tab_cases[i+1][j] == N_REGISTRED)
                    {
                        tab_cases[i+1][j] = tab_cases[i][j] + 1;
                        nb_cases--;
                    }
                    if (tab_cases[i][j-1] == N_REGISTRED)
                    {
                        tab_cases[i][j-1] = tab_cases[i][j] + 1;
                        nb_cases--;
                    }
                    if (tab_cases[i][j+1] == N_REGISTRED)
                    {
                        tab_cases[i][j+1] = tab_cases[i][j] + 1;
                        nb_cases--;
                    }
                    if (tab_cases[i-1][j-1] == N_REGISTRED)
                    {
                        tab_cases[i-1][j-1] = tab_cases[i][j] + sqrt(2);
                        nb_cases--;
                    }
                    if (tab_cases[i-1][j+1] == N_REGISTRED)
                    {
                        tab_cases[i-1][j+1] = tab_cases[i][j] + sqrt(2);
                        nb_cases--;
                    }
                    if (tab_cases[i+1][j-1] == N_REGISTRED)
                    {
                        tab_cases[i+1][j-1] = tab_cases[i][j] + sqrt(2);
                        nb_cases--;
                    }
                    if (tab_cases[i+1][j+1] == N_REGISTRED)
                    {
                        tab_cases[i+1][j+1] = tab_cases[i][j] + sqrt(2);
                        nb_cases--;
                    }
                }
            }
        }
    }

    return;
}

int trajectory(CtrlStruct *cvs, double goal_x, double goal_y)
{
	int goal_X = floor((goal_x + WIDTH/2.0) * CELL_X / WIDTH);
	int goal_Y = floor((goal_y + HEIGHT/2.0) * CELL_Y / HEIGHT);
	
	number_assigment(goal_X, goal_Y); //algorithme d'expansion
	
	int x = cvs->path->rob_pos_XY->x;
	int y = cvs->path->rob_pos_XY->y;
	
	int i, j;
	int somme[4] = {0};
	int direction;
	int minimum;
	goal_flag = 0;

	//Cette partie teste la somme des 6 case dans les 4 directions
	if (y>1)
	{
		for (j = y - 2; j < y; j++)
			for (i = x - 1; i <= x + 1; i++)
				somme[SENS_BAS] = somme[SENS_BAS] + tab_cases[i][j];
	}
	if (y<28)
	{
		for (j = y + 1; j < y + 3; j++)
			for (i = x - 1; i <= x + 1; i++)
				somme[SENS_HAUT] = somme[SENS_HAUT] + tab_cases[i][j];
	}
	if (x>1) // la premiere condition teste si c'est un bord 
	{
		for (i = x - 2; i < x; i++)
			for (j = y - 1; j <= y + 1; j++)
				somme[SENS_GAUCHE] = somme[SENS_GAUCHE] + tab_cases[i][j];
	}
	if (x<18)
	{
		for (i = x + 1; i < x + 3; i++)
			for (j = y - 1; j <= y + 1; j++)
				somme[SENS_DROITE] = somme[SENS_DROITE] + tab_cases[i][j];
	}
	
	//printf("x : %d , y: %d \n", x, y);
	minimum = somme[SENS_BAS];//On initialise la direction bas comme etant la plus petite par defaut
	direction = SENS_BAS;

	//Cette partie teste quel est le minimum parmi les sommes
	for (i = 0; i < 4; i++)
	{
		if (somme[i] < minimum)
		{
			minimum = somme[i];
			direction = i;
		}
		//printf("Somme1 = %d \n Somme2 = %d \n Somme3 = %d \n Somme4 = %d \n", somme[0], somme[1], somme[2], somme [3]);
	}

	if(direction != direction_pre)
		flag = TURN;
	direction_pre = direction;

	//printf("Goal_flag = %d \n", goal_flag);
	
	if (minimum > somme_pre)
	{
		goal_flag = 1;
		//printf("Goal_flag = %d \n", goal_flag);
		return 1;
	}

	somme_pre = minimum;
	
	if(goal_flag == 0) 
	{
		
		/*if (direction == -1)
		{
			if (turn(cvs, -M_PI/2.0, 1)) // alors tourner pour se mettre dans la direction -y
			{
				direction = SENS_BAS;
			}
		}*/
		switch (direction)
		{
		case SENS_BAS:
			switch(flag)
			{
				case TURN:
					if (turn(cvs,-M_PI/2.0, 1) == 1)
					{
						flag = RUN;
					}
					break;
				case RUN:
					if (run_y(cvs, Y_to_y(y-1)) == 1)
					{
						xy_to_XY(cvs);
						flag = TURN;
						trajectory(cvs, x, y);
						
					}
					break;
				case CONTINUE:
					
				flag = TURN;
				xy_to_XY(cvs);
				trajectory(cvs, x, y);
				break;
			}
				printf("bas \n");
				
			break;
			
		case SENS_HAUT:
		switch(flag)
			{
				case TURN:
					if (turn(cvs, M_PI/2.0, 1) == 1)
					{
						flag = RUN;
						
					}
					break;
				case RUN:
					if (run_y(cvs, Y_to_y(y+1)) == 1)
					{
						xy_to_XY(cvs);
						flag = TURN;
						trajectory(cvs, x, y);
					}
					break;
					case CONTINUE:
				flag = TURN;
				xy_to_XY(cvs);
				trajectory(cvs, x, y-1);
				break;
					
			}
			printf("haut \n");
			
			break;

		case SENS_GAUCHE:
		switch(flag)
			{
				case TURN:
					if (turn(cvs, -M_PI, 1) == 1)
					{
						flag = RUN;
						cvs->rob_pos->theta = M_PI;
					}
					break;
				case RUN:
					if (run_x(cvs, X_to_x(x-1)) == 1)
					{
						xy_to_XY(cvs);
						flag = TURN;
						trajectory(cvs, x, y);
					}
					break;
					case CONTINUE:
					xy_to_XY(cvs);
					flag = TURN;
					trajectory(cvs, x, y);
				break;
					
			}
			printf("gauche \n");
			
			break;

		case SENS_DROITE:
		switch(flag)
			{
				case TURN:
					if (turn(cvs, 0, 1) == 1)
					{
						flag = RUN;
					}
					break;
				case RUN:
					if (run_x(cvs, X_to_x(x+1)) == 1)
					{
						xy_to_XY(cvs);
						flag = TURN;
						trajectory(cvs, x, y);
					}
					break;
					case CONTINUE:
						xy_to_XY(cvs);
				flag = TURN;
					trajectory(cvs, x, y-1);
				break;
					
			}
		printf("droite \n");
				
			break;
		}
	}
	
	xy_to_XY(cvs);
	
	return 0;
}

void xy_to_XY(CtrlStruct *cvs)
{
	cvs->path->rob_pos_XY->x = floor((cvs->rob_pos->x * 1000 + WIDTH / 2) * CELL_X / WIDTH);
	cvs->path->rob_pos_XY->y = floor((cvs->rob_pos->y * 1000 + HEIGHT / 2) * CELL_Y / HEIGHT);
	
	return;
}

double X_to_x(int X)
{
	return (X-10)*0.1 + 0.05;
}

double Y_to_y(int Y)
{
	return (Y-15)*0.1 + 0.05;
}

/*
double X_to_x(int X)
{
	return (WIDTH * (X-10) / CELL_X - WIDTH/2.0) / 1000.0;
}

double Y_to_y(int Y)
{
	return (HEIGHT * (Y-15) / CELL_Y - HEIGHT/2.0) / 1000.0;
}
*/

/*
void update_path_planning(CtrlStruct *cvs)
{
	xy_to_XY(cvs);

	switch (cvs->path->goal_id)
	{
	case GOAL_01:

		if (cvs->path->rob_pos_XY == cvs->path->rob_goal_XY)
			cvs->path->goal_id = GOAL_02;

		break;

		// 			break;
		//
		// 			case GOAL_02:
		//
		// 				break;
	}
}
*/

NAMESPACE_CLOSE();
