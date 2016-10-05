/*! 
 * \author Nicolas Van der Noot
 * \file robot_id.h
 * \brief robot IDs
 */

#ifndef _ROBOT_ID_H_
#define _ROBOT_ID_H_

/// indexes of the robots
enum {ROBOT_B, ROBOT_R, ROBOT_Y, ROBOT_W, NB_TOT_ROBOTS};

/// indexes of the robot user derivatives
enum {UX_INDEX_0,
	UX_T1_B, UX_T2_B, UX_R3_B,
	UX_T1_R, UX_T2_R, UX_R3_R,
	UX_T1_Y, UX_T2_Y, UX_R3_Y,
	UX_T1_W, UX_T2_W, UX_R3_W};

// teams
enum {TEAM_A, TEAM_B, NB_TEAMS};

#endif
