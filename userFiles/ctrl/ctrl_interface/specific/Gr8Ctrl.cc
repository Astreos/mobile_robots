#include "Gr8Ctrl.hh"
#include "ctrl_main_gr8.h"
#include <iostream>

using namespace ctrlGr8;

/*! \brief constructor
 */
Gr8Ctrl::Gr8Ctrl(int robot_id, int nb_opp): Controller(GR8_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr8Ctrl::~Gr8Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr8Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr8Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr8Ctrl::close_controller()
{
	controller_finish(cvs);
}
