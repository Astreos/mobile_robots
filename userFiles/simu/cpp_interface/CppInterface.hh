/*! 
 * \author Nicolas Van der Noot
 * \file CppInterface.cc
 * \brief Main files for the interface with the C++ (header)
 */

#ifndef _CPP_INTERFACE_HH_
#define _CPP_INTERFACE_HH_

#include "mbs_data.h"
#include "user_realtime.h"
#include "RollingGestion.hh"
#include "ActuatorGestion.hh"
#include "GlobalGestion.hh"
#include <iostream>

/*! \brief Interface for the C++ code
 * 
 * The main code of the Robotran simulator is written in C.
 * This class is used to make the interface with the C++ code.
 * This class is stored as a 'void' in a C structure.
 * A static_cast is used to use it in C++.
 */
class CppInterface
{
	public:
		CppInterface(MbsData *mbs_data);
		~CppInterface();

		GlobalGestion* get_global_gestion() const { return global_gestion; }

		RollingGestion *get_rolling() const { return rolling; }
		ActuatorGestion *get_actuators() const { return actuators; }

		void compute();

	private:
		MbsData *mbs_data; ///< Robotran structure

		RollingGestion *rolling;    ///< rolling without slipping
		ActuatorGestion *actuators; ///< robot actuators

		GlobalGestion *global_gestion; ///< main gestion of the C++ code
};

#endif
