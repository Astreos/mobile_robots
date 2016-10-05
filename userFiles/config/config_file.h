/*! 
 * \author Nicolas Van der Noot
 * \file config_file.h
 * \brief This file is used to configure the project environment without parsing the 'config_file.txt' file
 *
 * To use it, you just have to uncomment 'HARDCODED_CONFIG'.
 * This file is inspired from 'config_file.txt'. You can read the instructions there.
 */
#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_

//#define HARDCODED_CONFIG ///< uncomment this field to use the hardcoded values instead of the ones of 'config_file.txt'

// ROBOT CONTROLLERS
#define BLUE_CTRL    "JoystickCtrl"
#define RED_CTRL     "RestingCtrl"
#define YELLLOW_CTRL "KeyboardCtrl"
#define WHITE_CTRL   "KeyboardCtrl"

// ROBOT INITIAL POSITIONS
#define BLUE_T1 0.67
#define BLUE_T2 1.15
#define BLUE_R3 -90.0

#define RED_T1 0.82
#define RED_T2 1.4
#define RED_R3 -90.0

#define YELLOW_T1 0.67
#define YELLOW_T2 -1.15
#define YELLOW_R3 90.0

#define WHITE_T1 0.82
#define WHITE_T2 -1.4
#define WHITE_R3 90.0

// ROBOTS INITIAL UNCERTAINTY
#define T1_UNCERT 0.02
#define T2_UNCERT 0.02
#define R3_UNCERT 10.0

// MOTORS
#define ACTUATOR_NOISE 0.07

#define WHEEL_FRICTION 0.2
#define TOWER_FRICTION 0.02

// SENSORS NOISE
#define WHEEL_NOISE 0.007
#define TOWER_NOISE 0.015

// POSITION BEACONS
#define TEAM_A_BEACON_1_X 1.062
#define TEAM_A_BEACON_1_Y 1.562

#define TEAM_A_BEACON_2_X -1.062
#define TEAM_A_BEACON_2_Y 1.562

#define TEAM_A_BEACON_3_X 0.0
#define TEAM_A_BEACON_3_Y -1.562

#define TEAM_B_BEACON_1_X 1.062
#define TEAM_B_BEACON_1_Y -1.562

#define TEAM_B_BEACON_2_X -1.062
#define TEAM_B_BEACON_2_Y -1.562

#define TEAM_B_BEACON_3_X 0.0
#define TEAM_B_BEACON_3_Y 1.562

// NUMBER OF JOINTS ANIM
#define NB_JOINTS_ANIM 48

#endif

