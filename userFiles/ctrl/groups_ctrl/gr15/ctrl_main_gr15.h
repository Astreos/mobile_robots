/*! 
 * \file ctrl_main_gr15.h
 * \brief main header of the controller
 */

#ifndef _CTRL_MAIN_GR15_H_
#define _CTRL_MAIN_GR15_H_

#include "CtrlStruct_gr15.h"
#include "namespace_ctrl.h"
#include <stdlib.h>

NAMESPACE_INIT(ctrlGr15);

void controller_init(CtrlStruct *cvs);
void controller_loop(CtrlStruct *cvs);
void controller_finish(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
