//
//-------------------------------------------------------------
//
//	ROBOTRAN - Version 6.6 (build : february 22, 2008)
//
//	Copyright 
//	Universite catholique de Louvain 
//	Departement de Mecanique 
//	Unite de Production Mecanique et Machines 
//	2, Place du Levant 
//	1348 Louvain-la-Neuve 
//	http://www.robotran.be// 
//
//	==> Generation Date : Sun Sep 11 16:30:54 2016
//
//	==> Project name : m454_project
//	==> using XML input file 
//
//	==> Number of joints : 24
//
//	==> Function : F 6 : Sensors Kinematical Informations (sens) 
//	==> Flops complexity : 12
//
//	==> Generation Time :  0.000 seconds
//	==> Post-Processing :  0.000 seconds
//
//-------------------------------------------------------------
//
 
#include <math.h> 

#include "mbs_data.h"
#include "mbs_project_interface.h"
#include "mbs_sensor.h"
 
void  mbs_sensor(MbsSensor *sens, 
              MbsData *s,
              int isens)
{ 
 
#include "mbs_sensor_m454_project.h" 
#define q s->q 
#define qd s->qd 
#define qdd s->qdd 
 
 

// === begin imp_aux === 

// === end imp_aux === 

// ===== BEGIN task 0 ===== 
 
// Sensor Kinematics 



// = = Block_0_0_0_0_0_1 = = 
 
// Trigonometric Variables  

  C3 = cos(q[3]);
  S3 = sin(q[3]);

// = = Block_0_0_0_0_0_5 = = 
 
// Trigonometric Variables  

  C9 = cos(q[9]);
  S9 = sin(q[9]);

// = = Block_0_0_0_0_0_9 = = 
 
// Trigonometric Variables  

  C15 = cos(q[15]);
  S15 = sin(q[15]);

// = = Block_0_0_0_0_0_13 = = 
 
// Trigonometric Variables  

  C21 = cos(q[21]);
  S21 = sin(q[21]);

// ====== END Task 0 ====== 

// ===== BEGIN task 1 ===== 
 
switch(isens)
{
 
// 
break;
case 1:
 


// = = Block_1_0_0_1_0_1 = = 
 
// Sensor Kinematics 


    POcp0_325 = s->dpt[3][1]+s->dpt[3][5];

// = = Block_1_0_0_1_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[1];
    sens->P[2] = q[2];
    sens->P[3] = POcp0_325;
    sens->R[1][1] = C3;
    sens->R[1][2] = S3;
    sens->R[2][1] = -S3;
    sens->R[2][2] = C3;
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[1];
    sens->V[2] = qd[2];
    sens->OM[3] = qd[3];
 
// 
break;
case 2:
 


// = = Block_1_0_0_2_0_5 = = 
 
// Sensor Kinematics 


    POcp1_326 = s->dpt[3][1]+s->dpt[3][9];

// = = Block_1_0_0_2_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[7];
    sens->P[2] = q[8];
    sens->P[3] = POcp1_326;
    sens->R[1][1] = C9;
    sens->R[1][2] = S9;
    sens->R[2][1] = -S9;
    sens->R[2][2] = C9;
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[7];
    sens->V[2] = qd[8];
    sens->OM[3] = qd[9];
 
// 
break;
case 3:
 


// = = Block_1_0_0_3_0_9 = = 
 
// Sensor Kinematics 


    POcp2_327 = s->dpt[3][13]+s->dpt[3][1];

// = = Block_1_0_0_3_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[13];
    sens->P[2] = q[14];
    sens->P[3] = POcp2_327;
    sens->R[1][1] = C15;
    sens->R[1][2] = S15;
    sens->R[2][1] = -S15;
    sens->R[2][2] = C15;
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[13];
    sens->V[2] = qd[14];
    sens->OM[3] = qd[15];
 
// 
break;
case 4:
 


// = = Block_1_0_0_4_0_13 = = 
 
// Sensor Kinematics 


    POcp3_328 = s->dpt[3][17]+s->dpt[3][1];

// = = Block_1_0_0_4_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[19];
    sens->P[2] = q[20];
    sens->P[3] = POcp3_328;
    sens->R[1][1] = C21;
    sens->R[1][2] = S21;
    sens->R[2][1] = -S21;
    sens->R[2][2] = C21;
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[19];
    sens->V[2] = qd[20];
    sens->OM[3] = qd[21];

break;
default:
break;
}


// ====== END Task 1 ====== 


}
 

