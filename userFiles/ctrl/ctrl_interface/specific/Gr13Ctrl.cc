#include "Gr13Ctrl.hh"
#include "ctrl_main_gr13.h"
#include <iostream>

using namespace ctrlGr13;

/*! \brief constructor
 */
Gr13Ctrl::Gr13Ctrl(int robot_id, int nb_opp): Controller(GR13_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr13Ctrl::~Gr13Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr13Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr13Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr13Ctrl::close_controller()
{
	controller_finish(cvs);
}
