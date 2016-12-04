#include "path_planning_gr18.h"
#include "init_pos_gr18.h"
#include "opp_pos_gr18.h"
#include "useful_gr18.h"
#include "path_regulation_gr18.h"
#include "speed_regulation_gr18.h"
#include <math.h>

#define RESOLUTION 2 //A ne pas changer tant que la prise en compte de l'empattement du robot n'est pas optimale
#define CELL_X (20*RESOLUTION + 2) //20 cases de base plus 1 case pour chaque bordure de plateau
#define CELL_Y (30*RESOLUTION + 2) //30 cases de base plus 1 case pour chaque bordure de plateau

#define N_REGISTRED 77
#define OBSTACLE 99
#define KEY_POINT 33
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
	
	// flags
	path->flag_trajectory = 0;
	path->flag_action = 0;
	path->count_actions = 1;

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
	
	//put_keypoints(cvs);
	
	assign_numbers(cvs);
	
	find_path(cvs);
	
	FILE* file = fopen("/home/nicolas/tableau.txt", "w");
	
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (path->map[i][j] < 10) {
				printf("%1.2f ", path->map[i][j]);
				fprintf(file, "%1.2f ", path->map[i][j]);
			}
			else {
				printf("%1.1f ", path->map[i][j]);
				fprintf(file, "%1.1f ", path->map[i][j]);
			}
		}
		printf("\n");
		fprintf(file, "\n");
	}
	
	/*
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (path->map[i][j] == OBSTACLE) {
				printf("o ");
				fprintf(file, "o ");
			}
			else if (path->map[i][j] == GOAL) {
				printf("* ");
				fprintf(file, "* ");
			}
			else if (path->map[i][j] == N_REGISTRED) {
				printf("+ ");
				fprintf(file, "+ ");
			}
			else if (path->map[i][j] == KEY_POINT) {
				printf("@ ");
				fprintf(file, "@ ");
			}
			else {
				printf(" ", path->map[i][j]);
				fprintf(file, " ", path->map[i][j]);
			}
		}
		printf("\n");
		fprintf(file, "\n");
	}
	*/
	
	fclose(file);
	
	path->flag_trajectory = 1;
	
	return;
}

void create_map(CtrlStruct *cvs)
{
	// variables declaration
	PathPlanning *path;
	int i, j;
	
	// variables initialization
	path = cvs->path;
	
	// --- Début du reset de la map --- //
	for (i = 0; i < CELL_X; i++)
	{
		for (j = 0; j < CELL_Y; j++)
		{
			path->map[i][j] = N_REGISTRED;
		}
	}
	// --- Fin de reset de la map --- //

	// --- Début de la création des obstacles sans la prise en compte de l'empattement du robot pour une résolution de 1 --- //
	
	for (i = 0; i < CELL_X; i++)
	{
		path->map[i][0] = OBSTACLE; // Bordure gauche
		path->map[i][CELL_Y-1] = OBSTACLE; // Bordure droite
	}
	for (j = 0; j < CELL_Y; j++)
	{
		path->map[0][j] = OBSTACLE; //Bordure supérieure
		path->map[CELL_X-1][j] = OBSTACLE; //Bordure inférieure
	}
	/*
	for (j = 1; j <= 5*RESOLUTION; j++)
	{
		path->map[15*RESOLUTION][j] = OBSTACLE; //Barrière en bas à gauche (camp jaune départ)
		path->map[15*RESOLUTION][CELL_Y-1-j] = OBSTACLE; //Barrière en bas à droite (camp bleu départ)
	}
	
	for (i = 1; i <= 5*RESOLUTION; i++)
	{
		path->map[i][7*RESOLUTION] = OBSTACLE; //Barrière en haut à gauche (camp bleu cible)
		path->map[i][CELL_Y-1-7*RESOLUTION] = OBSTACLE; //Barrière en haut à droite (camp jaune cible)
	}

	for (i = 10*RESOLUTION; i <= 12*RESOLUTION; i++)
	{
		for (j = 0; j < RESOLUTION; j++)
		{
			path->map[i][12*RESOLUTION + j] = OBSTACLE; //Barrière milieu-gauche
			path->map[i][19*RESOLUTION + j] = OBSTACLE; //Barrière milieu-droit
		}
	}
	for (i = 6*RESOLUTION; i <= 9*RESOLUTION; i++)
	{
		for (j = 0; j < 2*RESOLUTION; j++)
		{
			path->map[i][15*RESOLUTION + j] = OBSTACLE; // Barrière milieu-milieu verticale
		}
	}

	for (j = 12*RESOLUTION; j <= 19*RESOLUTION; j++)
	{
		for (i = 0; i < RESOLUTION; i++)
		{
			path->map[9*RESOLUTION + i][j] = OBSTACLE; // Barrière milieu horizontale
		}
	}
	*/
	// --- Fin de la création des obstacles sans la prise en compte de l'empattement du robot pour une résolution de 1 --- //
	
	// --- Début de la création des obstacles avec la prise en compte l'empattement du robot pour une résolution de 2 --- //
	
	for (i = 0; i <= CELL_X-6*RESOLUTION; i++) //On n'augmente pas les obstacles dans la base de départ
	{
		for (j = 0; j <= 3; j++)
		{
			path->map[i][j] = OBSTACLE; // Bordure gauche
			path->map[i][CELL_Y-1-j] = OBSTACLE; // Bordure droite
		}
	}
	for (j = 0; j <= CELL_Y-1; j++)
	{
		for (i = 0; i <= 3; i++)
		{
			path->map[i][j] = OBSTACLE; //Bordure supérieure
		}
	}
	for (j = 5*RESOLUTION+1; j < CELL_Y-1-5*RESOLUTION; j++) //On n'augmente pas les obstacles dans la base de départ
	{
		for (i = 0; i <= 3; i++)
		{
			path->map[CELL_X-1-i][j] = OBSTACLE; //Bordure inférieure
		}
	}
	
	for (j = 1; j <= 5*RESOLUTION+3; j++) //On n'augmente pas les obstacles dans la base de départ
	{
		for (i = -3; i <= 3; i++)
		{
			path->map[15*RESOLUTION-i][j] = OBSTACLE; // Barrière en bas à gauche (camp jaune départ)
			path->map[15*RESOLUTION-i][CELL_Y-1-j] = OBSTACLE; // Barrière en bas à droite (camp bleu départ)
		}
	}
	
	for (i = 1; i <= 5*RESOLUTION+3; i++)
	{
		for (j = -3; j <= 3; j++)
		{
			path->map[i][7*RESOLUTION+j] = OBSTACLE; //Barrière en haut à gauche (camp bleu cible)
			path->map[i][CELL_Y-1-7*RESOLUTION+j] = OBSTACLE; //Barrière en haut à droite (camp jaune cible)
		}
	}
	
	for (i = 19; i <= 27; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			path->map[i][20+j] = OBSTACLE; //Barrière milieu-gauche
			path->map[i][34+j] = OBSTACLE; //Barrière milieu-droit
		}
	}
	for (i = 8; i <= 16; i++)
	{
		for (j = 0; j <= 9; j++)
		{
			path->map[i][26+j] = OBSTACLE; //Barrière milieu-milieu verticale
		}
	}
	
	for (j = 0; j <= 21; j++)
	{
		for (i = 0; i <= 4; i++)
		{
			path->map[17+i][20+j] = OBSTACLE; // Barrière milieu horizontale
		}
	}
	
	// --- Fin de la création des obstacles avec la prise en compte l'empattement du robot pour une résolution de 2 --- //
	
	// --- Début de la création des obstacles avec la prise en compte l'empattement du robot pour une résolution de 4 --- //
	
	// --- Fin de la création des obstacles avec la prise en compte l'empattement du robot pour une résolution de 4 --- //
	
	return;
}

void put_keypoints(CtrlStruct *cvs)
{
	// variables declaration
	PathPlanning *path;
	int i, j;
	
	// variables initialization
	path = cvs->path;
	
	// --- Début du placement des keypoints pour une résolution de 2 --- //
	
	path->map[37][10] = KEY_POINT;
	path->map[37][10] = KEY_POINT;
	
	path->map[31][14] = KEY_POINT;
	path->map[27][14] = KEY_POINT;
	path->map[31][47] = KEY_POINT;
	path->map[26][47] = KEY_POINT;
	
	path->map[14][10] = KEY_POINT;
	path->map[14][18] = KEY_POINT;
	path->map[14][43] = KEY_POINT;
	path->map[14][51] = KEY_POINT;
	
	path->map[28][19] = KEY_POINT;
	path->map[28][28] = KEY_POINT;
	path->map[28][33] = KEY_POINT;
	path->map[28][42] = KEY_POINT;
	path->map[16][19] = KEY_POINT;
	path->map[16][42] = KEY_POINT;
	path->map[7][25] = KEY_POINT;
	path->map[7][36] = KEY_POINT;
	
	// --- Fin du placement des keypoints pour une résolution de 2 --- //
	
	return;
}

void assign_numbers(CtrlStruct *cvs)
{
	// variable declaration
	PathPlanning *path;
	int i, j, n, k, c, l;
	
	// variables initialization
	path = cvs->path;
	
	path->map[path->goal_XY->X][path->goal_XY->Y] = 0;
	/*
	k = 1;
	i = path->goal_XY->X;
	j = path->goal_XY->Y;
	c = 1;
	
	for(l = 0; l < 10; l++)
	{
		for (n = 0; n < k; n++)
		{
			if ((path->map[i-c][j-c+n+1] == N_REGISTRED) && (path->map[i-c+1][j-c+n+1] != OBSTACLE) && (path->map[i-c+1][j-c+n+1] != N_REGISTRED))
			{
				path->map[i-c][j-c+n+1] = path->map[i-c+1][j-c+n+1] + 1;
			}
			if ((path->map[i+c][j-c+n+1]) == N_REGISTRED && (path->map[i+c-1][j-c+n+1] != OBSTACLE) && (path->map[i+c-1][j-c+n+1] != N_REGISTRED))
			{
				path->map[i+c][j-c+n+1] = path->map[i+c-1][j-c+n+1] + 1;
			}
			if ((path->map[i-c+n+1][j-c] == N_REGISTRED) && (path->map[i-c+n+1][j-c+1] != OBSTACLE) && (path->map[i-c+n+1][j-c+1] != N_REGISTRED))
			{
				path->map[i-c+n+1][j-c] = path->map[i-c+n+1][j-c+1] + 1;
			}
			if ((path->map[i-c+n+1][j+c] == N_REGISTRED) && (path->map[i-c+n+1][j+c-1] != OBSTACLE) && (path->map[i-c+n+1][j+c-1] != N_REGISTRED))
			{
				path->map[i-c+n+1][j+c] = path->map[i-c+n+1][j+c-1] + 1;
			}
		}
		k += 2;
	
		if ((path->map[i-c][j-c] == N_REGISTRED) && (path->map[i-c+1][j-c+1] != OBSTACLE) && (path->map[i-c+1][j-c+1] != N_REGISTRED))
		{
			path->map[i-c][j-c] = path->map[i-c+1][j-c+1] + sqrt(2);
		}
		if ((path->map[i-c][j+c] == N_REGISTRED) && (path->map[i-c+1][j+c-1] != OBSTACLE) && (path->map[i-c+1][j+c-1] != N_REGISTRED))
		{
			path->map[i-c][j+c] = path->map[i-c+1][j+c-1] + sqrt(2);
		}
		if ((path->map[i+c][j-c] == N_REGISTRED) && (path->map[i+c-1][j-c+1] != OBSTACLE) && (path->map[i+c-1][j-c+1] != N_REGISTRED))
		{
			path->map[i+c][j-c] = path->map[i+c-1][j-c+1] + sqrt(2);
		}
		if ((path->map[i+c][j+c] == N_REGISTRED) && (path->map[i+c-1][j+c-1] != OBSTACLE) && (path->map[i+c-1][j+c-1] != N_REGISTRED))
		{
			path->map[i+c][j+c] = path->map[i+c-1][j+c-1] + sqrt(2);
		}
		c += 1;
	}
	*/
    
    for(n = path->rob_pos_XY->X + 1; n > 0; n--)
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

int manage_keypoints(CtrlStruct *cvs, int KP_X, int KP_Y)
{
	// variable declaration
	PathPlanning *path;
	int i, j, n, k, c, l;
	
	// variables initialization
	path = cvs->path;
	
	k = 1;
	c = 1;
	
	for (n = 0; n < k; n++)
	{
		if ((path->map[KP_X-c][KP_Y-c+n+1] == N_REGISTRED) && (path->map[KP_X-c+1][KP_Y-c+n+1] != OBSTACLE) && (path->map[KP_X-c+1][KP_Y-c+n+1] != N_REGISTRED))
		{
			path->map[KP_X-c][KP_Y-c+n+1] = path->map[KP_X-c+1][KP_Y-c+n+1] + 1;
		}
		if ((path->map[KP_X+c][KP_Y-c+n+1]) == N_REGISTRED && (path->map[KP_X+c-1][KP_Y-c+n+1] != OBSTACLE) && (path->map[KP_X+c-1][KP_Y-c+n+1] != N_REGISTRED))
		{
			path->map[KP_X+c][KP_Y-c+n+1] = path->map[KP_X+c-1][KP_Y-c+n+1] + 1;
		}
		if ((path->map[KP_X-c+n+1][KP_Y-c] == N_REGISTRED) && (path->map[KP_X-c+n+1][KP_Y-c+1] != OBSTACLE) && (path->map[KP_X-c+n+1][KP_Y-c+1] != N_REGISTRED))
		{
			path->map[KP_X-c+n+1][KP_Y-c] = path->map[KP_X-c+n+1][KP_Y-c+1] + 1;
		}
		if ((path->map[KP_X-c+n+1][KP_Y+c] == N_REGISTRED) && (path->map[KP_X-c+n+1][KP_Y+c-1] != OBSTACLE) && (path->map[KP_X-c+n+1][KP_Y+c-1] != N_REGISTRED))
		{
			path->map[KP_X-c+n+1][KP_Y+c] = path->map[KP_X-c+n+1][KP_Y+c-1] + 1;
		}
	}
	k += 2;
	
	if ((path->map[KP_X-c][KP_Y-c] == N_REGISTRED) && (path->map[KP_X-c+1][KP_Y-c+1] != OBSTACLE) && (path->map[KP_X-c+1][KP_Y-c+1] != N_REGISTRED))
	{
		path->map[KP_X-c][KP_Y-c] = path->map[KP_X-c+1][KP_Y-c+1] + sqrt(2);
	}
	if ((path->map[KP_X-c][KP_Y+c] == N_REGISTRED) && (path->map[KP_X-c+1][KP_Y+c-1] != OBSTACLE) && (path->map[KP_X-c+1][KP_Y+c-1] != N_REGISTRED))
	{
		path->map[KP_X-c][KP_Y+c] = path->map[KP_X-c+1][KP_Y+c-1] + sqrt(2);
	}
	if ((path->map[KP_X+c][KP_Y-c] == N_REGISTRED) && (path->map[KP_X+c-1][KP_Y-c+1] != OBSTACLE) && (path->map[KP_X+c-1][KP_Y-c+1] != N_REGISTRED))
	{
		path->map[KP_X+c][KP_Y-c] = path->map[KP_X+c-1][KP_Y-c+1] + sqrt(2);
	}
	if ((path->map[KP_X+c][KP_Y+c] == N_REGISTRED) && (path->map[KP_X+c-1][KP_Y+c-1] != OBSTACLE) && (path->map[KP_X+c-1][KP_Y+c-1] != N_REGISTRED))
	{
		path->map[KP_X+c][KP_Y+c] = path->map[KP_X+c-1][KP_Y+c-1] + sqrt(2);
	}
	c += 1;
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
	
	path->list_goal[0][0] = path->rob_pos_XY->X;
	path->list_goal[0][1] = path->rob_pos_XY->Y;
	
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
	return (X - (CELL_X-2)/RESOLUTION - 0.5)/(10.0 * RESOLUTION);
}

double Y_to_y(int Y)
{
	return (Y - (CELL_Y-2)/RESOLUTION - 0.5)/(10.0 * RESOLUTION);
}

NAMESPACE_CLOSE();
