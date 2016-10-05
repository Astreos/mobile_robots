/*! 
 * \file ctrl_main_gr13.h
 * \brief main header of the controller
 */

#ifndef _CTRL_MAIN_GR13_H_
#define _CTRL_MAIN_GR13_H_

#include "CtrlStruct_gr13.h"
#include "namespace_ctrl.h"
#include <stdlib.h>

NAMESPACE_INIT(ctrlGr13);

void controller_init(CtrlStruct *cvs);
void controller_loop(CtrlStruct *cvs);
void controller_finish(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
