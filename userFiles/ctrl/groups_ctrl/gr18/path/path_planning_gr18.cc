#include "path_planning_gr18.h"
#include "init_pos_gr18.h"
#include "opp_pos_gr18.h"
#include "useful_gr18.h"
#include "path_regulation_gr18.h"
#include "speed_regulation_gr18.h"
#include <math.h>
#include <iostream>

enum { GOAL_01, GOAL_02, GOAL_03 };
enum { RUN, TURN, CONTINUE};

#define WIDTH 2000
#define HEIGHT 3000

#define CELL_X 20
#define CELL_Y 30

#define FIRST_GOAL_X 0.0
#define FIRST_GOAL_Y 0.0

#define AJUSTMENT 0.008
#define N_REGISTRED -1
#define OBSTACLE  99

#define SENS_BAS 0
#define SENS_HAUT 1
#define SENS_GAUCHE 2
#define SENS_DROITE 3

static int sens = SENS_BAS;
static int tab_cases[20][30] = { { 0 },{ 0 } };
static int goal_flag = 0;
static int flag = TURN;


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
	
	/*
	for (j = 0; j<CELL_Y; j++)
	{
		for (i = 0; i<CELL_X; i++)
			printf("%d ", tab_cases[i][j]);
		printf("\n");
	}
	printf("\n\n");
	*/

	number_assigment(x, y); //algorithme d'expansion
	
	/*
	for (j = 0; j<CELL_Y; j++)
	{
		for (i = 0; i<CELL_X; i++)
			printf("%d ", tab_cases[i][j]);
		printf("\n");
	}
	printf("\n\n");
	* */

	//trajectory(6, 13); //test

	path->rob_pos_XY = (RobotPosition*)malloc(sizeof(RobotPosition));
	path->rob_goal_XY = (RobotPosition*)malloc(sizeof(RobotPosition));

	path->rob_pos_XY = robposition;

	path->rob_goal_XY->x = FIRST_GOAL_X;
	path->rob_goal_XY->y = FIRST_GOAL_Y;

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

void mapcreating()
{
	int i = 0; int j = 0;

	for (i = 0; i < CELL_X; i++)
		for (j = 0; j < CELL_Y; j++)
			tab_cases[i][j] = N_REGISTRED;

	for (i = 0; i <= 4; i++)
	{
		tab_cases[14][i] = OBSTACLE;     // Barrière en haut à droite (vertical)
		tab_cases[14][25 + i] = OBSTACLE;     // Barrière en bas à droite (vertical)
	}

	for (i = 0; i <= 4; i++) // Barrière en haut à droite (horizonthal)
	{
		tab_cases[i][6] = OBSTACLE;
		tab_cases[i][23] = OBSTACLE;
	}

	for (i = 8; i <= 11; i++) // Barrière milieu-bas (horizonthal)
	{
		tab_cases[i][18] = OBSTACLE;
		tab_cases[i][11] = OBSTACLE;
	}

	for (i = 12; i <= 18; i++) // Barrière milieu (vertical)
		tab_cases[8][i] = OBSTACLE;

	for (i = 5; i <= 8; i++)
	{
		tab_cases[i][14] = OBSTACLE; // Barrière milieu-milieu (horizonthal)
		tab_cases[i][15] = OBSTACLE;
	}
}

void number_assigment(int x, int y)
{
	int i, j;
	
	int value = 0;
	tab_cases[x][y] = value;

	int cases_ni = 559; // 20 * 30 - 40 obstacle - la case du goal = 559
	
	while (cases_ni) // tant qu'on n'a pas enregistré toute les cases
	{
		for (i = 0; i <= 19; i++)	//Cher en memoire faut peu etre trouver un autre moyen de chopper toutes les cases qui viennent d'etre changés sans un double for aussi gros
		{
			for (j = 0; j <= 29; j++)
			{
				if (tab_cases[i][j] == value)
				{
					if (i>0 && tab_cases[i - 1][j] == N_REGISTRED) // la premiere condition teste si c'est un bord 
					{												  //les obstacles sont deja registred donc pas besoin d'un deuxieme test pour eux
						tab_cases[i - 1][j] = value + 1;
						cases_ni--;
					}
					if (i<19 && tab_cases[i + 1][j] == N_REGISTRED)
					{
						tab_cases[i + 1][j] = value + 1;
						cases_ni--;
					}
					if (j>0 && tab_cases[i][j - 1] == N_REGISTRED)
					{
						tab_cases[i][j - 1] = value + 1;
						cases_ni--;
					}
					if (j <29 && tab_cases[i][j + 1] == N_REGISTRED)
					{
						tab_cases[i][j + 1] = value + 1;
						cases_ni--;
					}
				}
			}
		}
		value++;// toute les cases avec value ont eté testé on passe a la value suivante
	}
	return;
}

int trajectory(CtrlStruct *cvs, int x, int y)
{
	/*
	x = cvs->path->rob_pos_XY->x;
	y = cvs->path->rob_pos_XY->y;
	*/
	
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

	printf("Minimum = %d \n", minimum);
	printf("Somme_pre = %d \n", somme_pre);
	
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
					//printf("%d \n", turn(cvs, -M_PI/2.0, 1));
					if (turn(cvs, -M_PI/2.0, 1) == 1)
					{
						flag = RUN;
					}
					break;
				case RUN:
				//printf("%f \n", Y_to_y(y-1));
					if (run_y(cvs, Y_to_y(y-1)) == 1)
					{
						//flag = CONTINUE;
						trajectory(cvs, x, y-1);
					}
					break;
				case CONTINUE:
				flag = TURN;
					trajectory(cvs, x, y-1);
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
						flag = TURN;
						trajectory(cvs, x, y+1);
					}
					break;
					case CONTINUE:
				flag = TURN;
					trajectory(cvs, x, y-1);
				break;
					
			}
			printf("haut \n");
			
			break;

		case SENS_GAUCHE:
		switch(flag)
			{
				case TURN:
					if (turn(cvs, M_PI, 1) == 1)
					{
						flag = RUN;
					}
					break;
				case RUN:
					if (run_y(cvs, X_to_x(x-1)) == 1)
					{
						flag = TURN;
						trajectory(cvs, x-1, y);
					}
					break;
					case CONTINUE:
				flag = TURN;
					trajectory(cvs, x, y-1);
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
					if (run_y(cvs, X_to_x(x+1)) == 1)
					{
						flag = TURN;
						trajectory(cvs, x+1, y);
					}
					break;
					case CONTINUE:
				flag = TURN;
					trajectory(cvs, x, y-1);
				break;
					
			}
		printf("droite \n");
				
			break;
		}
	}
	
	return 0;
	//stocker la somme, appeleer le truc de deplacement ou stocker la direction
}

void xy_to_XY(CtrlStruct *cvs)
{
	cvs->path->rob_pos_XY->x = floor((cvs->rob_pos->x * 1000 + WIDTH / 2) * CELL_X / WIDTH);
	cvs->path->rob_pos_XY->y = floor((cvs->rob_pos->y * 1000 + HEIGHT / 2) * CELL_Y / HEIGHT);

	//printf("robpos xy: %f,%f , en XY : %f,%f\n",cvs->rob_pos->x,cvs->rob_pos->y, cvs->path->rob_goal_XY->x,cvs->path->rob_goal_XY->y);

}

double X_to_x(int X)
{
	return (X-10)*0.1 - 0.05;
}

double Y_to_y(int Y)
{
	return (Y-15)*0.1 - 0.05;
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

NAMESPACE_CLOSE();
