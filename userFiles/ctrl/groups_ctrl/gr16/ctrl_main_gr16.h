/*! 
 * \file ctrl_main_gr16.h
 * \brief main header of the controller
 */

#ifndef _CTRL_MAIN_GR16_H_
#define _CTRL_MAIN_GR16_H_

#include "CtrlStruct_gr16.h"
#include "namespace_ctrl.h"
#include <stdlib.h>

NAMESPACE_INIT(ctrlGr16);

void controller_init(CtrlStruct *cvs);
void controller_loop(CtrlStruct *cvs);
void controller_finish(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
