#include "Gr16Ctrl.hh"
#include "ctrl_main_gr16.h"
#include <iostream>

using namespace ctrlGr16;

/*! \brief constructor
 */
Gr16Ctrl::Gr16Ctrl(int robot_id, int nb_opp): Controller(GR16_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr16Ctrl::~Gr16Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr16Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr16Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr16Ctrl::close_controller()
{
	controller_finish(cvs);
}
