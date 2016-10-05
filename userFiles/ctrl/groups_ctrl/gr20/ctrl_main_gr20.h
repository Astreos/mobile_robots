/*! 
 * \file ctrl_main_gr20.h
 * \brief main header of the controller
 */

#ifndef _CTRL_MAIN_GR20_H_
#define _CTRL_MAIN_GR20_H_

#include "CtrlStruct_gr20.h"
#include "namespace_ctrl.h"
#include <stdlib.h>

NAMESPACE_INIT(ctrlGr20);

void controller_init(CtrlStruct *cvs);
void controller_loop(CtrlStruct *cvs);
void controller_finish(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
