/*! 
 * \file ctrl_main_gr4.h
 * \brief main header of the controller
 */

#ifndef _CTRL_MAIN_GR4_H_
#define _CTRL_MAIN_GR4_H_

#include "CtrlStruct_gr4.h"
#include "namespace_ctrl.h"
#include <stdlib.h>

NAMESPACE_INIT(ctrlGr4);

void controller_init(CtrlStruct *cvs);
void controller_loop(CtrlStruct *cvs);
void controller_finish(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
