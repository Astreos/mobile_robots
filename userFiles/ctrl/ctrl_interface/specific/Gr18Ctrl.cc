#include "Gr18Ctrl.hh"
#include "ctrl_main_gr18.h"
#include <iostream>

using namespace ctrlGr18;

/*! \brief constructor
 */
Gr18Ctrl::Gr18Ctrl(int robot_id, int nb_opp): Controller(GR18_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr18Ctrl::~Gr18Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr18Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr18Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr18Ctrl::close_controller()
{
	controller_finish(cvs);
}
