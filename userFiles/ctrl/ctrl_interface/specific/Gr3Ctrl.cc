#include "Gr3Ctrl.hh"
#include "ctrl_main_gr3.h"
#include <iostream>

using namespace ctrlGr3;

/*! \brief constructor
 */
Gr3Ctrl::Gr3Ctrl(int robot_id, int nb_opp): Controller(GR3_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr3Ctrl::~Gr3Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr3Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr3Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr3Ctrl::close_controller()
{
	controller_finish(cvs);
}
