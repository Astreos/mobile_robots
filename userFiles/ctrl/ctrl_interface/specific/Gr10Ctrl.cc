#include "Gr10Ctrl.hh"
#include "ctrl_main_gr10.h"
#include <iostream>

using namespace ctrlGr10;

/*! \brief constructor
 */
Gr10Ctrl::Gr10Ctrl(int robot_id, int nb_opp): Controller(GR10_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr10Ctrl::~Gr10Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr10Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr10Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr10Ctrl::close_controller()
{
	controller_finish(cvs);
}
