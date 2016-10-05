/*! 
 * \author Group 18
 * \file mcl_gr18.h
 * \brief localization using Monte Carlo technic
 */

#ifndef _MCL_GR18_H_
#define _MCL_GR18_H_

#include "CtrlStruct_gr18.h"
#include "init_pos_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/// Markov main structure
struct MCLStruct
{
	int dummy_variable; ///< put your own variable, this is just an example without purpose
};

void mcl_update(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
