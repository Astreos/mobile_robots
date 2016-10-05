/*! 
 * \author Nicolas Van der Noot
 * \file namespace_ctrl.h
 * \brief Header file used to set a namespace region, using macros
 */

#ifndef _NAMESPACE_CTRL_HH_
#define _NAMESPACE_CTRL_HH_

#ifdef SIMU_PROJECT
	#define NAMESPACE_INIT(name_controller) namespace name_controller{
	#define NAMESPACE_CLOSE() }
#else
	#define NAMESPACE_INIT(name_controller)
	#define NAMESPACE_CLOSE() 
#endif

#endif
