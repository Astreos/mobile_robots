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
	//variables declaration
	PathPlanning *path;
	
	int i, j;
	
	// memory allocation
	path = (PathPlanning*) malloc(sizeof(PathPlanning));
	if (path == NULL) {exit(0);}
	
	// ----- path-planning initialization start ----- //
	
	// map
	path->map = (int**) malloc(CELL_X*sizeof(int*));
	if (path->map == NULL) {exit(0);}
	
	//initialise the map
	for(i=0; i<CELL_X; i++)
	{
		path->map[i] = (int*) malloc(CELL_Y*sizeof(int));
		if (path->map[i] == NULL) {exit(0);}
		
		for(j=0; j<CELL_Y; j++)
		{
			path->map[i][j] = N_REGISTRED;
		}
	}
	
	// flag
	path->flag_trajectory = false;
	
	// rob_pos_XY
	path->rob_pos_XY = (PositionXY*) malloc(sizeof(PositionXY*));
	if (path->rob_pos_XY == NULL) {exit(0);}
	
	// goal_XY
	path->goal_XY = (PositionXY*) malloc(sizeof(PositionXY*));
	if (path->goal_XY == NULL) {exit(0);}
	
	// list_checkpoints
	path->size_list_checkpoints = 25*RESOLUTION;
	
	path->list_checkpoints = (int**) malloc(path->size_list_checkpoints*sizeof(int*));
	if (path->list_checkpoints == NULL) {exit(0);}

	for(i=0; i<path->size_list_checkpoints; i++)
	{
		path->list_checkpoints[i] = (int*) malloc(2*sizeof(int));
		if (path->list_checkpoints[i] == NULL) {exit(0);}

		for(j=0; j<2; j++)
		{
			path->list_checkpoints[i][j] = 0;
		}
	}
	
	// number of checkpoints
	path->nb_checkpoints = 0;
	
	// current checkpoint
	path->current_checkpoint = 0;
	
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
	// variables declaration
	int i;
	
	// ----- path-planning memory release start ----- //
	
	// map
	for(i = 0; i < CELL_X; i++)
	{
		free(path->map[i]);
	}
	free(path->map);
	
	// rob_pos_XY
	free(path->rob_pos_XY);
	
	// goal_XY
	free(path->goal_XY);
	
	// list_checkpoints
	for(i = 0; i < path->size_list_checkpoints; i++)
	{
		free(path->list_checkpoints[i]);
	}
	free(path->list_checkpoints);
	
	free(path);
	
	// ----- path-planning memory release end ----- //
	
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
	
	//printf("BEFORE MANAGE_OPP \n");
	
	manage_opp(cvs, 0); // put high values in the cases next to the opponant
	
	if (path->map[path->goal_XY->X][path->goal_XY->Y] == OPPONENT)
	{
		create_map(cvs);
	}
	
	//printf("BEFORE ASSIGN_NUMBERS \n");
	
	assign_numbers(cvs); // Expension algorithm for the case, used for path planning
	
	if (path->map[path->rob_pos_XY->X][path->rob_pos_XY->Y] > 90)
	{
		create_map(cvs);
		
		assign_numbers(cvs);
	}
	
	/*
	FILE* file = fopen("../../last_map.txt", "w");
	
	// Printf to see the path on terminal
	for (i = 0; i<CELL_X; i++)
	{
		for (j = 0; j<CELL_Y; j++)
		{
			if (path->map[i][j] < 10) {
				fprintf(file, " %d ", path->map[i][j]);
			}
			else if (path->map[i][j] == OBSTACLE) {
				fprintf(file, "oo ");
			}
			else if (path->map[i][j] == GOAL) {
				fprintf(file, "@@ ");
			}
			else if (path->map[i][j] == OPPONENT) {
				fprintf(file, "** ");
			}
			else {
				fprintf(file, "%d ", path->map[i][j]);
			}
		}
		fprintf(file, "\n\n");
	}
	
	fclose(file);
	
	//printf("BEFORE FIND_PATH \n");
	*/
	
	find_path(cvs);
	
	path->flag_trajectory = true;
	
	/*
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
	*/
	
	return;
}

void create_map(CtrlStruct *cvs)
{
	// variables declaration
	PathPlanning *path;
	
	int i, j;
	
	// variables initialization
	path = cvs->path;
	
	// --- D�but du reset de la map --- //
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
	// --- D�but de la cr�ation des obstacles sans la prise en compte de l'empattement du robot pour une r�solution de 1 --- //
		for (i = 0; i < CELL_X; i++) // Bordures lat�rales
		{
			path->map[i][0] = OBSTACLE; // Bordure gauche
			path->map[i][CELL_Y-1] = OBSTACLE; // Bordure droite
		}
		for (j = 0; j < CELL_Y; j++) // Bordures extremales
		{
			path->map[0][j] = OBSTACLE; // Bordure sup�rieure
			path->map[CELL_X-1][j] = OBSTACLE; // Bordure inf�rieure
		}
		
		for (j = 1; j <= 5*RESOLUTION; j++) // Barri�res adjacentes aux camps de d�part
		{
			path->map[15*RESOLUTION][j] = OBSTACLE; // Barri�re en bas � gauche (camp jaune d�part)
			path->map[15*RESOLUTION][CELL_Y-1-j] = OBSTACLE; // Barri�re en bas � droite (camp bleu d�part)
		}
		
		for (i = 1; i <= 5*RESOLUTION; i++) // Barri�res adjacentes aux camps d'arriv�e
		{
			path->map[i][7*RESOLUTION] = OBSTACLE; // Barri�re en haut � gauche (camp bleu cible)
			path->map[i][CELL_Y-1-7*RESOLUTION] = OBSTACLE; // Barri�re en haut � droite (camp jaune cible)
		}
		
		for (i = 10*RESOLUTION; i <= 12*RESOLUTION; i++) // Barri�res lat�rales du milieu
		{
			for (j = 0; j < RESOLUTION; j++)
			{
				path->map[i][12*RESOLUTION + j] = OBSTACLE; // Barri�re milieu-gauche
				path->map[i][19*RESOLUTION + j] = OBSTACLE; // Barri�re milieu-droit
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
	// --- Fin de la cr�ation des obstacles sans la prise en compte de l'empattement du robot pour une r�solution de 1 --- //
	}
	else if (RESOLUTION == 2)
	{
	// --- D�but de la cr�ation des obstacles avec la prise en compte l'empattement du robot pour une r�solution de 2 --- //
		for (i = 0; i <= CELL_X-6*RESOLUTION; i++) // Construction des bordures lat�rales augment�es de 3, sauf au niveau des camps de d�part
		{
			for (j = 0; j <= 3; j++)
			{
				path->map[i][j] = OBSTACLE; // Bordure gauche
				path->map[i][CELL_Y-1-j] = OBSTACLE; // Bordure droite
			}
		}
		for (i = CELL_X-6*RESOLUTION+1; i < CELL_X; i++) // Construction des bordures lat�rales non augment�es au niveau des camps de d�part
		{
			path->map[i][0] = OBSTACLE; // Bordure gauche
			path->map[i][CELL_Y-1] = OBSTACLE; // Bordure droite
		}
		
		for (j = 0; j < CELL_Y; j++) // Construction de la bordure sup�rieure augment�e de 3
		{
			for (i = 0; i <= 3; i++)
			{
				path->map[i][j] = OBSTACLE; // Bordure sup�rieure
			}
		}
		for (j = 5*RESOLUTION+1; j < CELL_Y-1-5*RESOLUTION; j++) // Construction de la bordure inf�rieure augment�e de 3, sauf au niveau des camps de d�part
		{
			for (i = 0; i <= 3; i++)
			{
				path->map[CELL_X-1-i][j] = OBSTACLE; // Bordure inf�rieure
			}
		}
		for (j = 0; j < 5*RESOLUTION+1; j++) // Construction des bordures extremales non augment�es au niveau des camps de d�part
		{
			path->map[CELL_X-1][j] = OBSTACLE; // Bordure inf�rieure camp jaune
			path->map[CELL_X-1][CELL_Y-1-j] = OBSTACLE; // Bordure inf�rieure camp bleu
		}
		
		for (j = 2*RESOLUTION; j <= 5*RESOLUTION+4; j++) // Construction des barri�res adjacentes aux camps de d�part augment�es de 4
		{
			for (i = -4; i <= 3; i++)
			{
				path->map[15*RESOLUTION+i][j] = OBSTACLE; // Barri�re en bas � gauche (camp jaune d�part)
				path->map[15*RESOLUTION+i][CELL_Y-1-j] = OBSTACLE; // Barri�re en bas � droite (camp bleu d�part)
			}
			
		}
		path->map[12*RESOLUTION+1][5*RESOLUTION+5] = OBSTACLE;
		path->map[12*RESOLUTION+1][CELL_Y-1-(5*RESOLUTION+5)] = OBSTACLE;
		path->map[13*RESOLUTION][5*RESOLUTION+5] = OBSTACLE;
		path->map[13*RESOLUTION][CELL_Y-1-(5*RESOLUTION+5)] = OBSTACLE;
		path->map[12*RESOLUTION+1][5*RESOLUTION+4] = OBSTACLE;
		path->map[12*RESOLUTION+1][CELL_Y-1-(5*RESOLUTION+4)] = OBSTACLE;
		
		for (i = 2*RESOLUTION; i <= 5*RESOLUTION+4; i++) // Construction des barri�res adjacentes aux camps d'arriv�e augment�es de 3 en �paisseur, et 4 en longueur
		{
			for (j = -3; j <= 0; j++)
			{
				path->map[i][6*RESOLUTION+1+j] = OBSTACLE; // Moiti� gauche de la barri�re en haut � gauche (camp bleu cible)
				path->map[i][CELL_Y-1-(6*RESOLUTION+1+j)] = OBSTACLE; // Moiti� droite de la barri�re en haut � droite (camp jaune cible)
			}
			for (j = 0; j <= 2; j++)
			{
				path->map[i][7*RESOLUTION+j] = OBSTACLE; // Moiti� droite de la barri�re en haut � gauche (camp bleu cible)
				path->map[i][CELL_Y-1-(7*RESOLUTION+j)] = OBSTACLE; // Moiti� gauche de la barri�re en haut � droite (camp jaune cible)
			}
		}
		
		for (i = 9*RESOLUTION+1; i <= 12*RESOLUTION+4; i++) // Construction des barri�res lat�rales du milieu augment�es de 4 � l'ext�rieur, et 3 � l'int�rieur
		{
			for (j = 12*RESOLUTION-1-4; j <=  12*RESOLUTION+3; j++)
			{
				path->map[i][j] = OBSTACLE; // Barri�re milieu-gauche
				path->map[i][CELL_Y-1-j] = OBSTACLE; // Barri�re milieu-droit
			}
		}
		path->map[12*RESOLUTION+5][12*RESOLUTION-1-5] = OBSTACLE; // On augmente le coin gauche de la barri�re milieu-gauche de 1
		path->map[12*RESOLUTION+5][CELL_Y-1-(12*RESOLUTION-1-5)] = OBSTACLE; // On augmente le coin droit de la barri�re milieu-droit de 1
		path->map[12*RESOLUTION+5][12*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barri�re milieu-gauche de 1
		path->map[12*RESOLUTION+5][CELL_Y-1-(12*RESOLUTION-1-4)] = OBSTACLE; // On augmente le coin droit de la barri�re milieu-droit de 1
		path->map[12*RESOLUTION+4][12*RESOLUTION-1-5] = OBSTACLE; // On augmente le coin gauche de la barri�re milieu-gauche de 1
		path->map[12*RESOLUTION+4][CELL_Y-1-(12*RESOLUTION-1-5)] = OBSTACLE; // On augmente le coin droit de la barri�re milieu-droit de 1
		
		for (i = 8*RESOLUTION+1-4; i <= 9*RESOLUTION; i++) // Construction de la barri�re horizontale du milieu augment�es de 4
		{
			for (j = 12*RESOLUTION-1-4; j <= 19*RESOLUTION+4; j++)
			{
				path->map[i][j] = OBSTACLE; // Barri�re milieu horizontale
			}
		}
		path->map[9*RESOLUTION-1-5][12*RESOLUTION-1-5] = OBSTACLE; // On augmente le coin gauche de la barri�re horizontale de 1
		path->map[9*RESOLUTION-1-5][CELL_Y-1-(12*RESOLUTION-1-5)] = OBSTACLE; // On augmente le coin droit de la barri�re horizontale de 1
		path->map[9*RESOLUTION-1-4][12*RESOLUTION-1-5] = OBSTACLE; // On augmente le coin gauche de la barri�re horizontale de 1
		path->map[9*RESOLUTION-1-4][CELL_Y-1-(12*RESOLUTION-1-5)] = OBSTACLE; // On augmente le coin droit de la barri�re horizontale de 1
		path->map[9*RESOLUTION-1-5][12*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barri�re horizontale de 1
		path->map[9*RESOLUTION-1-5][CELL_Y-1-(12*RESOLUTION-1-4)] = OBSTACLE; // On augmente le coin droit de la barri�re horizontale de 1
		
		for (i = 6*RESOLUTION-1-4; i <= 6*RESOLUTION; i++) // Construction de la barri�re verticale du milieu augment�es de 4
		{
			for (j = 15*RESOLUTION-1-4; j <= 16*RESOLUTION+4; j++)
			{
				path->map[i][j] = OBSTACLE; // Barri�re milieu-milieu verticale
			}
		}
		path->map[6*RESOLUTION-1-5][15*RESOLUTION-1-5] = OBSTACLE; // On augmente le coin gauche de la barri�re verticale de 1
		path->map[6*RESOLUTION-1-5][CELL_Y-1-(15*RESOLUTION-1-5)] = OBSTACLE; // On augmente le coin droit de la barri�re verticale de 1
		path->map[6*RESOLUTION-1-5][15*RESOLUTION-1-4] = OBSTACLE; // On augmente le coin gauche de la barri�re verticale de 1
		path->map[6*RESOLUTION-1-5][CELL_Y-1-(15*RESOLUTION-1-4)] = OBSTACLE; // On augmente le coin droit de la barri�re verticale de 1
		path->map[6*RESOLUTION-1-4][15*RESOLUTION-1-5] = OBSTACLE; // On augmente le coin gauche de la barri�re verticale de 1
		path->map[6*RESOLUTION-1-4][CELL_Y-1-(15*RESOLUTION-1-5)] = OBSTACLE; // On augmente le coin droit de la barri�re verticale de 1
	// --- Fin de la cr�ation des obstacles avec la prise en compte l'empattement du robot pour une r�solution de 2 --- //
	}
	else if (RESOLUTION == 4)
	{
	// --- D�but de la cr�ation des obstacles avec la prise en compte l'empattement du robot pour une r�solution de 4 --- //
	
	// --- Fin de la cr�ation des obstacles avec la prise en compte l'empattement du robot pour une r�solution de 4 --- //
	}
	
	return;
}

// Mets une valeur haute aux cases occup�s par le robot adverse
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


//Algorythme d'expansion
void assign_numbers(CtrlStruct *cvs)
{
	// variables declaration
	PathPlanning *path;
	
	int i, j, value, cases_ni;
	
	// variables initialization
	path = cvs->path;
	
	value = 0;
	cases_ni = 0;
	
	path->map[path->goal_XY->X][path->goal_XY->Y] = 0;

	//Compte le nombre de cases a assigner
	for (i = 0; i < CELL_X; i++)
	{
		for (j = 0; j < CELL_Y; j++)
		{
			if (path->map[i][j] == N_REGISTRED)
			{
				cases_ni += 1;
			}
		}
	}

	//Assigne les cases avec l'algorythme d'exmpension jusqu'a ateindre le robot
	while ((cases_ni > 0) && (path->map[path->rob_pos_XY->X][path->rob_pos_XY->Y] > 90)) // tant qu'on n'a pas enregistr� toute les cases
	{
		for (i = 1; i < CELL_X-1; i++)
		{
			for (j = 1; j < CELL_Y-1; j++)
			{
				if (path->map[i][j] == value)
				{
					if ((path->map[i-1][j] == N_REGISTRED) || ((path->map[i-1][j] == OBSTACLE) && (i-1 == path->rob_pos_XY->X) && (j == path->rob_pos_XY->Y)))
					{
						path->map[i-1][j] = value + 1;
						cases_ni--;
					}
					if ((path->map[i+1][j] == N_REGISTRED) || ((path->map[i+1][j] == OBSTACLE) && (i+1 == path->rob_pos_XY->X) && (j == path->rob_pos_XY->Y)))
					{
						path->map[i+1][j] = value + 1;
						cases_ni--;
					}
					if ((path->map[i][j-1] == N_REGISTRED) || ((path->map[i][j-1] == OBSTACLE) && (i == path->rob_pos_XY->X) && (j-1 == path->rob_pos_XY->Y)))
					{
						path->map[i][j-1] = value + 1;
						cases_ni--;
					}
					if ((path->map[i][j+1] == N_REGISTRED) || ((path->map[i][j+1] == OBSTACLE) && (i == path->rob_pos_XY->X) && (j+1 == path->rob_pos_XY->Y)))
					{
						path->map[i][j+1] = value + 1;
						cases_ni--;
					}
					
					if ((path->map[i-1][j-1] == N_REGISTRED) || ((path->map[i-1][j-1] == OBSTACLE) && (i-1 == path->rob_pos_XY->X) && (j-1 == path->rob_pos_XY->Y)))
					{
						path->map[i-1][j-1] = value + 1;
						cases_ni--;
					}
					if ((path->map[i+1][j-1] == N_REGISTRED) || ((path->map[i+1][j-1] == OBSTACLE) && (i+1 == path->rob_pos_XY->X) && (j-1 == path->rob_pos_XY->Y)))
					{
						path->map[i+1][j-1] = value + 1;
						cases_ni--;
					}
					if ((path->map[i+1][j+1] == N_REGISTRED) || ((path->map[i+1][j+1] == OBSTACLE) && (i+1 == path->rob_pos_XY->X) && (j+1 == path->rob_pos_XY->Y)))
					{
						path->map[i+1][j+1] = value + 1;
						cases_ni--;
					}
					if ((path->map[i-1][j+1] == N_REGISTRED) || ((path->map[i-1][j+1] == OBSTACLE) && (i-1 == path->rob_pos_XY->X) && (j+1 == path->rob_pos_XY->Y)))
					{
						path->map[i-1][j+1] = value + 1;
						cases_ni--;
					}
				}
			}
		}
		value++;// toute les cases avec value ont et� test� on passe a la value suivante
	}
	
	return;
}


// Trouve le chemin en faisant les sommes des cases autour du robot et en choisissant la direction avec la somme la plus basse
void find_path(CtrlStruct *cvs)
{
	// variable declaration
	PathPlanning *path;
	
	int i, j, i_line, j_line, i_diag, j_diag, k, l, c;
	float avg_temp, avg_line, avg_diag;
	
	// variables initialization
	path = cvs->path;
	
	k = 0;
	
	path->list_checkpoints[0][0] = path->rob_pos_XY->X;
	path->list_checkpoints[0][1] = path->rob_pos_XY->Y;
	
	while (path->map[path->list_checkpoints[k][0]][path->list_checkpoints[k][1]] >= 1)
	{
		k++;
		
		avg_line = 100.0;
		
		avg_temp = 0;
		c = 0;
		for (l = 0; l <= 3; l++)
		{
			if (path->map[path->list_checkpoints[k-1][0]+l][path->list_checkpoints[k-1][1]] != OBSTACLE)
			{
				avg_temp += path->map[path->list_checkpoints[k-1][0]+l][path->list_checkpoints[k-1][1]];
				c += 1;
			}
		}
		avg_temp = avg_temp/c;
		if ((avg_temp < avg_line) && (path->map[path->list_checkpoints[k-1][0]+1][path->list_checkpoints[k-1][1]] != OBSTACLE))
		{
			avg_line = avg_temp;
			i_line = 1;
			j_line = 0;
		}
		
		avg_temp = 0;
		c = 0;
		for (l = 0; l <= 3; l++)
		{
			if (path->map[path->list_checkpoints[k-1][0]-l][path->list_checkpoints[k-1][1]] != OBSTACLE)
			{
				avg_temp += path->map[path->list_checkpoints[k-1][0]-l][path->list_checkpoints[k-1][1]];
				c += 1;
			}
		}
		avg_temp = avg_temp/c;
		if ((avg_temp < avg_line) && (path->map[path->list_checkpoints[k-1][0]-1][path->list_checkpoints[k-1][1]] != OBSTACLE))
		{
			avg_line = avg_temp;
			i_line = -1;
			j_line = 0;
		}
		
		avg_temp = 0;
		c = 0;
		for (l = 0; l <= 3; l++)
		{
			if (path->map[path->list_checkpoints[k-1][0]][path->list_checkpoints[k-1][1]+l] != OBSTACLE)
			{
				avg_temp += path->map[path->list_checkpoints[k-1][0]][path->list_checkpoints[k-1][1]+l];
				c += 1;
			}
		}
		avg_temp = avg_temp/c;
		if ((avg_temp < avg_line) && (path->map[path->list_checkpoints[k-1][0]][path->list_checkpoints[k-1][1]+1] != OBSTACLE))
		{
			avg_line = avg_temp;
			i_line = 0;
			j_line = 1;
		}
		
		avg_temp = 0;
		c = 0;
		for (l = 0; l <= 3; l++)
		{
			if (path->map[path->list_checkpoints[k-1][0]][path->list_checkpoints[k-1][1]-l] != OBSTACLE)
			{
				avg_temp += path->map[path->list_checkpoints[k-1][0]][path->list_checkpoints[k-1][1]-l];
				c += 1;
			}
		}
		avg_temp = avg_temp/c;
		if ((avg_temp < avg_line) && (path->map[path->list_checkpoints[k-1][0]][path->list_checkpoints[k-1][1]-1] != OBSTACLE))
		{
			avg_line = avg_temp;
			i_line = 0;
			j_line = -1;
		}
		
		avg_diag = 100.0;
		
		avg_temp = 0;
		c = 0;
		for (l = 0; l <= 3; l++)
		{
			if (path->map[path->list_checkpoints[k-1][0]-l][path->list_checkpoints[k-1][1]-l] != OBSTACLE)
			{
				avg_temp += path->map[path->list_checkpoints[k-1][0]-l][path->list_checkpoints[k-1][1]-l];
				c += 1;
			}
		}
		avg_temp = avg_temp/c;
		if ((avg_temp < avg_diag) && (path->map[path->list_checkpoints[k-1][0]-1][path->list_checkpoints[k-1][1]-1] != OBSTACLE))
		{
			avg_diag = avg_temp;
			i_diag = -1;
			j_diag = -1;
		}
		
		avg_temp = 0;
		c = 0;
		for (l = 0; l <= 3; l++)
		{
			if (path->map[path->list_checkpoints[k-1][0]+l][path->list_checkpoints[k-1][1]+l] != OBSTACLE)
			{
				avg_temp += path->map[path->list_checkpoints[k-1][0]+l][path->list_checkpoints[k-1][1]+l];
				c += 1;
			}
		}
		avg_temp = avg_temp/c;
		if ((avg_temp < avg_diag) && (path->map[path->list_checkpoints[k-1][0]+1][path->list_checkpoints[k-1][1]+1] != OBSTACLE))
		{
			avg_diag = avg_temp;
			i_diag = 1;
			j_diag = 1;
		}
		
		avg_temp = 0;
		c = 0;
		for (l = 0; l <= 3; l++)
		{
			if (path->map[path->list_checkpoints[k-1][0]+l][path->list_checkpoints[k-1][1]-l] != OBSTACLE)
			{
				avg_temp += path->map[path->list_checkpoints[k-1][0]+l][path->list_checkpoints[k-1][1]-l];
				c += 1;
			}
		}
		avg_temp = avg_temp/c;
		if ((avg_temp < avg_diag) && (path->map[path->list_checkpoints[k-1][0]+1][path->list_checkpoints[k-1][1]-1] != OBSTACLE))
		{
			avg_diag = avg_temp;
			i_diag = 1;
			j_diag = -1;
		}
		
		avg_temp = 0;
		c = 0;
		for (l = 0; l <= 3; l++)
		{
			if (path->map[path->list_checkpoints[k-1][0]-l][path->list_checkpoints[k-1][1]+l] != OBSTACLE)
			{
				avg_temp += path->map[path->list_checkpoints[k-1][0]-l][path->list_checkpoints[k-1][1]+l];
				c += 1;
			}
		}
		avg_temp = avg_temp/c;
		if ((avg_temp < avg_diag) && (path->map[path->list_checkpoints[k-1][0]-1][path->list_checkpoints[k-1][1]+1] != OBSTACLE))
		{
			avg_diag = avg_temp;
			i_diag = -1;
			j_diag = 1;
		}
		
		if (avg_diag < avg_line)
		{
			i = i_diag;
			j = j_diag;
		}
		else
		{
			i = i_line;
			j = j_line;
		}
		
		if (k > path->size_list_checkpoints-1)
		{
			path->list_checkpoints = (int**) realloc(path->list_checkpoints, (path->size_list_checkpoints+1)*sizeof(int*));
			if (path->list_checkpoints == NULL) {exit(0);}
			
			path->list_checkpoints[k] = (int*) malloc(2*sizeof(int));
			if (path->list_checkpoints[k] == NULL) {exit(0);}
			
			path->size_list_checkpoints++;
		}
		
		path->list_checkpoints[k][0] = path->list_checkpoints[k-1][0]+i;
		path->list_checkpoints[k][1] = path->list_checkpoints[k-1][1]+j;
	}
	
	path->nb_checkpoints = k;
	
	/*
	for (i = 0; i <= path->nb_checkpoints; i++)
	{
		path->map[path->list_checkpoints[i][0]][path->list_checkpoints[i][1]] = GOAL;
	}
	*/
	
	return;
}

// Calcule les coordon�s mm en coordon�es de cases
int x_to_X(double x)
{
	return RESOLUTION*floor(10.0*x + 1/RESOLUTION) + (CELL_X - 2)/RESOLUTION;
}

// Calcule les coordon�s mm en coordon�es de cases
int y_to_Y(double y)
{
	return RESOLUTION*floor(10.0*y + 1/RESOLUTION) + (CELL_Y - 2)/RESOLUTION;
}

// Calcule les coordon�s de cases en coordon�s mm
double X_to_x(int X)
{
	return (X - (CELL_X-2-1)/RESOLUTION) / (10.0 * RESOLUTION);
}
// Calcule les coordon�s de cases en coordon�s mm
double Y_to_y(int Y)
{
	return (Y - (CELL_Y-2-1)/RESOLUTION) / (10.0 * RESOLUTION);
}

NAMESPACE_CLOSE();
