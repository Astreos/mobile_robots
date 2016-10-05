#include "Gr17Ctrl.hh"
#include "ctrl_main_gr17.h"
#include <iostream>

using namespace ctrlGr17;

/*! \brief constructor
 */
Gr17Ctrl::Gr17Ctrl(int robot_id, int nb_opp): Controller(GR17_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr17Ctrl::~Gr17Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr17Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr17Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr17Ctrl::close_controller()
{
	controller_finish(cvs);
}
