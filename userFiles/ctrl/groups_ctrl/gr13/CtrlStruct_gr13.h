/*! 
 * \file CtrlStruct_gr13.h
 * \brief Controller main structure
 */

#ifndef _CTRL_STRUCT_GR13_H_
#define _CTRL_STRUCT_GR13_H_

#include "ctrl_io.h"
#include "namespace_ctrl.h"
#include <stdlib.h>

NAMESPACE_INIT(ctrlGr13);

/// Main controller structure
typedef struct CtrlStruct
{
	CtrlIn *inputs;   ///< controller inputs
	CtrlOut *outputs; ///< controller outputs

} CtrlStruct;

// function prototypes
CtrlStruct* init_CtrlStruct(CtrlIn *inputs, CtrlOut *outputs);
void free_CtrlStruct(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
