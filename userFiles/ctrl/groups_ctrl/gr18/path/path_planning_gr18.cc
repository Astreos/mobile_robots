#include "path_planning_gr18.h"
#include "init_pos_gr18.h"
#include "opp_pos_gr18.h"
#include "useful_gr18.h"
#include "path_regulation_gr18.h"
#include "speed_regulation_gr18.h"
#include "strategy_gr18.h"
#include <math.h>

#define RESOLUTION 2 //Fonctionnel pour 2 seulement
#define CELL_X (20*RESOLUTION + 2) //20 cases de base plus 1 case pour chaque bordure de plateau
#define CELL_Y (30*RESOLUTION + 2) //30 cases de base plus 1 case pour chaque bordure de plateau

#define N_REGISTRED 97
#define OBSTACLE 99
#define GOAL 98
#define OPPONENT 95

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
	path->flag_trajectory = 0;
	
	// current action
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
	for(i=0; i<=path->nb_goals; i++)
	{
		free(path->list_goal[i]);
	}
	free(path->list_goal);

	// rob_pos_XY
	free(path->rob_pos_XY);
	
	// goal_XY
	free(path->goal_XY);
	
	free(path);
	
	// ----- path-planning memory release end ----- //
	
	return;
}

void trajectory(CtrlStruct *cvs, double goal_x, double goal_y)
{
	// variables declaration
	RobotPosition *rob_pos;
	PathPlanning *path;
	Strategy *strat;
	
	int i, j;
    
    // variables initialization
    rob_pos = cvs->rob_pos;
    path = cvs->path;
	strat  = cvs->strat;
    
    path->rob_pos_XY->X = x_to_X(rob_pos->x);
	path->rob_pos_XY->Y = y_to_Y(rob_pos->y);
	path->goal_XY->X = x_to_X(goal_x);
	path->goal_XY->Y = y_to_Y(goal_y);
	
	create_map(cvs);
	
	manage_opp(cvs, 1);
	
	if (path->map[path->goal_XY->X][path->goal_XY->Y] == OPPONENT && strat->list_targets[strat->nb_targets-1] != strat->list_targets[strat->nb_targets-2])
	{
		strat->nb_targets += 1;
		
		strat->list_targets = (int*) realloc(strat->list_targets, strat->nb_targets*sizeof(int));
		if (strat->list_targets == NULL) {exit(0);}
		
		strat->list_targets[strat->nb_targets-1] = strat->list_targets[strat->current_action];
		
		strat->current_action += 1;
		
		return;
	}
	
	assign_numbers(cvs);
	
	if ((path->goal_XY->X != N_REGISTRED) && (path->goal_XY->Y != N_REGISTRED))
	{
		find_path(cvs);
		path->flag_trajectory = 1;
	}
	else
	{
		return;
	}
	
	FILE* file = fopen("../../last_map.txt", "w");
	
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (path->map[i][j] < 10) {
				fprintf(file, "%1.2f ", path->map[i][j]);
			}
			else if (path->map[i][j] == OBSTACLE) {
				fprintf(file, "oooo ");
			}
			else if (path->map[i][j] == GOAL) {
				fprintf(file, "@@@@ ");
			}
			else if (path->map[i][j] == OPPONENT) {
				fprintf(file, "**** ");
			}
			else {
				fprintf(file, "%1.1f ", path->map[i][j]);
			}
		}
		fprintf(file, "\n\n");
	}
	
	fclose(file);
	
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (path->map[i][j] == OBSTACLE) {
				printf("o ");
			}
			else if (path->map[i][j] == GOAL) {
				printf("@ ");
			}
			else if (path->map[i][j] == OPPONENT) {
				printf("* ");
			}
			else if (path->map[i][j] == N_REGISTRED) {
				printf("+ ");
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}
	printf("\n");
	
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

	if (RESOLUTION == 1)
	{
	// --- Début de la création des obstacles sans la prise en compte de l'empattement du robot pour une résolution de 1 --- //
		for (i = 0; i < CELL_X; i++) // Bordures latérales
		{
			path->map[i][0] = OBSTACLE; // Bordure gauche
			path->map[i][CELL_Y-1] = OBSTACLE; // Bordure droite
		}
		for (j = 0; j < CELL_Y; j++) // Bordures extremales
		{
			path->map[0][j] = OBSTACLE; // Bordure supérieure
			path->map[CELL_X-1][j] = OBSTACLE; // Bordure inférieure
		}
		
		for (j = 1; j <= 5*RESOLUTION; j++) // Barrières adjacentes aux camps de départ
		{
			path->map[15*RESOLUTION][j] = OBSTACLE; // Barrière en bas à gauche (camp jaune départ)
			path->map[15*RESOLUTION][CELL_Y-1-j] = OBSTACLE; // Barrière en bas à droite (camp bleu départ)
		}
		
		for (i = 1; i <= 5*RESOLUTION; i++) // Barrières adjacentes aux camps d'arrivée
		{
			path->map[i][7*RESOLUTION] = OBSTACLE; // Barrière en haut à gauche (camp bleu cible)
			path->map[i][CELL_Y-1-7*RESOLUTION] = OBSTACLE; // Barrière en haut à droite (camp jaune cible)
		}
		
		for (i = 10*RESOLUTION; i <= 12*RESOLUTION; i++) // Barrières latérales du milieu
		{
			for (j = 0; j < RESOLUTION; j++)
			{
				path->map[i][12*RESOLUTION + j] = OBSTACLE; // Barrière milieu-gauche
				path->map[i][19*RESOLUTION + j] = OBSTACLE; // Barrière milieu-droit
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
	// --- Fin de la création des obstacles sans la prise en compte de l'empattement du robot pour une résolution de 1 --- //
	}
	else if (RESOLUTION == 2)
	{
	// --- Début de la création des obstacles avec la prise en compte l'empattement du robot pour une résolution de 2 --- //
		for (i = 0; i <= CELL_X-6*RESOLUTION; i++) // Construction des bordures latérales augmentées de 3, sauf au niveau des camps de départ
		{
			for (j = 0; j <= 3; j++)
			{
				path->map[i][j] = OBSTACLE; // Bordure gauche
				path->map[i][CELL_Y-1-j] = OBSTACLE; // Bordure droite
			}
		}
		for (i = CELL_X-6*RESOLUTION+1; i < CELL_X; i++) // Construction des bordures latérales non augmentées au niveau des camps de départ
		{
			path->map[i][0] = OBSTACLE; // Bordure gauche
			path->map[i][CELL_Y-1] = OBSTACLE; // Bordure droite
		}
		
		for (j = 0; j < CELL_Y; j++) // Construction de la bordure supérieure augmentée de 3
		{
			for (i = 0; i <= 3; i++)
			{
				path->map[i][j] = OBSTACLE; // Bordure supérieure
			}
		}
		for (j = 5*RESOLUTION+1; j < CELL_Y-1-5*RESOLUTION; j++) // Construction de la bordure inférieure augmentée de 3, sauf au niveau des camps de départ
		{
			for (i = 0; i <= 3; i++)
			{
				path->map[CELL_X-1-i][j] = OBSTACLE; // Bordure inférieure
			}
		}
		for (j = 0; j < 5*RESOLUTION+1; j++) // Construction des bordures extremales non augmentées au niveau des camps de départ
		{
			path->map[CELL_X-1][j] = OBSTACLE; // Bordure inférieure camp jaune
			path->map[CELL_X-1][CELL_Y-1-j] = OBSTACLE; // Bordure inférieure camp bleu
		}
		
		for (j = 2*RESOLUTION; j <= 5*RESOLUTION+3; j++) // Construction des barrières adjacentes aux camps de départ augmentées de 3
		{
			for (i = -3; i <= 3; i++)
			{
				path->map[15*RESOLUTION+i][j] = OBSTACLE; // Barrière en bas à gauche (camp jaune départ)
				path->map[15*RESOLUTION+i][CELL_Y-1-j] = OBSTACLE; // Barrière en bas à droite (camp bleu départ)
			}
			
		}
		for (j = 5*RESOLUTION+4; j <= 5*RESOLUTION+5; j++) // On augmente le pic de chaque barrière adjacente aux camps de départ de 2
		{
			path->map[15*RESOLUTION][j] = OBSTACLE; // Barrière en bas à gauche (camp jaune départ)
			path->map[15*RESOLUTION][CELL_Y-1-j] = OBSTACLE; // Barrière en bas à droite (camp bleu départ)
			
		}
		path->map[13*RESOLUTION][5*RESOLUTION+4] = OBSTACLE;
		path->map[13*RESOLUTION][CELL_Y-1-(5*RESOLUTION+4)] = OBSTACLE;
		path->map[13*RESOLUTION+1][5*RESOLUTION+4] = OBSTACLE;
		path->map[13*RESOLUTION+1][CELL_Y-1-(5*RESOLUTION+4)] = OBSTACLE;
		path->map[13*RESOLUTION][5*RESOLUTION+3] = OBSTACLE;
		path->map[13*RESOLUTION][CELL_Y-1-(5*RESOLUTION+3)] = OBSTACLE;
		
		for (i = 2*RESOLUTION; i <= 5*RESOLUTION+3; i++) // Construction des barrières adjacentes aux camps d'arrivée augmentées de 3
		{
			for (j = -2; j <= 0; j++)
			{
				path->map[i][6*RESOLUTION+1+j] = OBSTACLE; // Moitié gauche de la barrière en haut à gauche (camp bleu cible)
				path->map[i][CELL_Y-1-(6*RESOLUTION+1+j)] = OBSTACLE; // Moitié gauche de la barrière en haut à droite (camp jaune cible)
			}
			for (j = 0; j <= 2; j++)
			{
				path->map[i][7*RESOLUTION+j] = OBSTACLE; // Moitié droite de la barrière en haut à gauche (camp bleu cible)
				path->map[i][CELL_Y-1-(7*RESOLUTION+j)] = OBSTACLE; // Moitié droite de la barrière en haut à droite (camp jaune cible)
			}
		}
		for (i = 5*RESOLUTION+4; i <= 5*RESOLUTION+7; i++) // On augmente le pic de chaque barrière adjacente aux camps cibles de 3
		{
			path->map[i][6*RESOLUTION+1] = OBSTACLE; // Moitié gauche de la barrière en haut à gauche (camp bleu cible)
			path->map[i][CELL_Y-1-(6*RESOLUTION+1)] = OBSTACLE; // Moitié gauche de la barrière en haut à droite (camp jaune cible)
			path->map[i][7*RESOLUTION] = OBSTACLE; // Moitié droite de la barrière en haut à gauche (camp bleu cible)
			path->map[i][CELL_Y-1-7*RESOLUTION] = OBSTACLE; // Moitié droite de la barrière en haut à droite (camp jaune cible)
		}
		
		for (i = 9*RESOLUTION-1-3; i <= 12*RESOLUTION+3; i++) // Construction des barrières latérales du milieu augmentées de 3
		{
			for (j = 12*RESOLUTION-1-3; j <=  12*RESOLUTION+3; j++)
			{
				path->map[i][j] = OBSTACLE; // Barrière milieu-gauche
				path->map[i][CELL_Y-1-j] = OBSTACLE; // Barrière milieu-droit
			}
		}
		for (i = 14*RESOLUTION; i <= 14*RESOLUTION-1+3; i++) // On augmente le pic de chaque barrière latérale du milieu de 3
		{
			path->map[i][12*RESOLUTION-1] = OBSTACLE; // Moitié gauche de la barrière milieu-gauche
			path->map[i][CELL_Y-1-(12*RESOLUTION-1)] = OBSTACLE; // Moitié gauche de la barrière milieu-droite
			path->map[i][12*RESOLUTION] = OBSTACLE; // Moitié droite de la barrière milieu-gauche
			path->map[i][CELL_Y-1-12*RESOLUTION] = OBSTACLE; // Moitié droite de la barrière milieu-droite
		}
		path->map[12*RESOLUTION+4][12*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barrière milieu-gauche de 1
		path->map[12*RESOLUTION+4][CELL_Y-1-(12*RESOLUTION-1-4)] = OBSTACLE; // On augmente le coin droit de la barrière milieu-droite de 1
		path->map[12*RESOLUTION+3][12*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barrière milieu-gauche de 1
		path->map[12*RESOLUTION+3][CELL_Y-1-(12*RESOLUTION-1-4)] = OBSTACLE; // On augmente le coin droit de la barrière milieu-droite de 1
		path->map[12*RESOLUTION+4][12*RESOLUTION-1-3] = OBSTACLE; // On augmente le coin gauche de la barrière milieu-gauche de 1
		path->map[12*RESOLUTION+4][CELL_Y-1-(12*RESOLUTION-1-3)] = OBSTACLE; // On augmente le coin droit de la barrière milieu-droite de 1
		
		for (j = 13*RESOLUTION-1; j <= 18*RESOLUTION; j++) // Construction de la barrière horizontale du milieu augmentées de 3
		{
			for (i = 9*RESOLUTION-1-3; i <= 9*RESOLUTION; i++)
			{
				path->map[i][j] = OBSTACLE; // Barrière milieu horizontale
			}
		}
		path->map[9*RESOLUTION-1-4][12*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barrière horizontale de 1
		path->map[9*RESOLUTION-1-4][CELL_Y-1-(12*RESOLUTION-1-4)] = OBSTACLE; // On augmente le coin droit de la barrière horizontale de 1
		path->map[9*RESOLUTION-1-3][12*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barrière horizontale de 1
		path->map[9*RESOLUTION-1-3][CELL_Y-1-(12*RESOLUTION-1-4)] = OBSTACLE; // On augmente le coin droit de la barrière horizontale de 1
		path->map[9*RESOLUTION-1-4][12*RESOLUTION-1-3] = OBSTACLE; // On augmente le coin gauche de la barrière horizontale de 1
		path->map[9*RESOLUTION-1-4][CELL_Y-1-(12*RESOLUTION-1-3)] = OBSTACLE; // On augmente le coin droit de la barrière horizontale de 1
		
		for (i = 4*RESOLUTION; i <= 7*RESOLUTION-1; i++) // Construction de la barrière verticale du milieu augmentées de 3
		{
			for (j = 15*RESOLUTION-1-3; j <= 16*RESOLUTION+3; j++)
			{
				path->map[i][j] = OBSTACLE; // Barrière milieu-milieu verticale
			}
		}
		path->map[4*RESOLUTION-1][15*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barrière verticale de 1
		path->map[4*RESOLUTION-1][CELL_Y-1-(15*RESOLUTION-1-4)] = OBSTACLE; // On augmente le pic du coin droit de la barrière verticale de 1
		path->map[4*RESOLUTION][15*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barrière verticale de 1
		path->map[4*RESOLUTION][CELL_Y-1-(15*RESOLUTION-1-4)] = OBSTACLE; // On augmente le pic du coin droit de la barrière verticale de 1
		path->map[4*RESOLUTION-1][15*RESOLUTION-1-3] = OBSTACLE; // On augmente le coin gauche de la barrière verticale de 1
		path->map[4*RESOLUTION-1][CELL_Y-1-(15*RESOLUTION-1-3)] = OBSTACLE; // On augmente le pic du coin droit de la barrière verticale de 1
		
	// --- Fin de la création des obstacles avec la prise en compte l'empattement du robot pour une résolution de 2 --- //
	}
	else if (RESOLUTION == 4)
	{
	// --- Début de la création des obstacles avec la prise en compte l'empattement du robot pour une résolution de 4 --- //
	
	// --- Fin de la création des obstacles avec la prise en compte l'empattement du robot pour une résolution de 4 --- //
	}
	
	return;
}

void manage_opp(CtrlStruct *cvs, int delta)
{
	// variable declaration
	PathPlanning *path;
	OpponentsPosition *opp_pos;
	RobotPosition *rob_pos;
	int i, j, k;
	int i_start, i_end, j_start, j_end;
	
	// variables initialization
	path = cvs->path;
	opp_pos = cvs->opp_pos;
	rob_pos = cvs->rob_pos;
	
	for (k = 0; k < opp_pos->nb_opp; k++)
	{
		if (sqrt(pow(rob_pos->x - opp_pos->x[k],2) + pow(rob_pos->y - opp_pos->y[k],2)) > 0.50)
		{
			i_start = x_to_X(opp_pos->x[k])-4-delta;
			i_end = x_to_X(opp_pos->x[k])+4+delta;
			j_start = y_to_Y(opp_pos->y[k])-4-delta;
			j_end = y_to_Y(opp_pos->y[k])+4+delta;
			
			if (i_start < 0)
			{
				i_start = 0;
			}
			else if (i_end >= CELL_X)
			{
				i_end = CELL_X-1;
			}
			
			if (j_start < 0)
			{
				j_start = 0;
			}
			else if (j_end >= CELL_Y)
			{
				j_end = CELL_Y-1;
			}
			
			for (i = i_start; i <= i_end; i++)
			{
				for (j = j_start; j <= j_end; j++)
				{
					if (path->map[i][j] != OBSTACLE)
					{
						path->map[i][j] = OPPONENT;
					}
				}
			}
		}
		else
		{
			if (path->map[x_to_X(opp_pos->x[k])][y_to_Y(opp_pos->y[k])] != OBSTACLE)
			{
				path->map[x_to_X(opp_pos->x[k])][y_to_Y(opp_pos->y[k])] = OPPONENT;
			}
		}
	}
	
	return;
}

void assign_numbers(CtrlStruct *cvs)
{
	// variables declaration
	PathPlanning *path;
	int i, j, n;
	
	// variables initialization
	path = cvs->path;
	
	path->map[path->goal_XY->X][path->goal_XY->Y] = 0;	
    
    for(n = 50; n > 0; n--)
	{
		for (i = 0; i < CELL_X; i++)
		{
			for (j = 0; j < CELL_Y; j++)
			{
				if (path->map[i][j] != N_REGISTRED && path->map[i][j] != OBSTACLE && path->map[i][j] != OPPONENT)
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

					if (path->map[i-1][j+1] == N_REGISTRED)
					{
						path->map[i-1][j+1] = path->map[i][j] + sqrt(2);
					}
					if (path->map[i+1][j-1] == N_REGISTRED)
					{
						path->map[i+1][j-1] = path->map[i][j] + sqrt(2);
					}
					if (path->map[i-1][j-1] == N_REGISTRED)
					{
						path->map[i-1][j-1] = path->map[i][j] + sqrt(2);
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
					
					if (k >= 10*RESOLUTION)
					{						
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
	return RESOLUTION*floor(10.0*x + 1/RESOLUTION) + (CELL_X - 2)/RESOLUTION;
}

int y_to_Y(double y)
{
	return RESOLUTION*floor(10.0*y + 1/RESOLUTION) + (CELL_Y - 2)/RESOLUTION;
}

double X_to_x(int X)
{
	return (X - (CELL_X-2-1)/RESOLUTION) / (10.0 * RESOLUTION);
}

double Y_to_y(int Y)
{
	return (Y - (CELL_Y-2-1)/RESOLUTION) / (10.0 * RESOLUTION);
}

NAMESPACE_CLOSE();
