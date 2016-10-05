/*! 
 * \author Group 18
 * \file controller_main_gr18.h
 * \brief main header of the controller
 */

#ifndef _CTRL_MAIN_GR18_H_
#define _CTRL_MAIN_GR18_H_

#include "CtrlStruct_gr18.h"
#include "namespace_ctrl.h"
#include <stdlib.h>

NAMESPACE_INIT(ctrlGr18);

void controller_init(CtrlStruct *cvs);
void controller_loop(CtrlStruct *cvs);
void controller_finish(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
