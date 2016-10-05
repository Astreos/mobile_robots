#include "Gr7Ctrl.hh"
#include "ctrl_main_gr7.h"
#include <iostream>

using namespace ctrlGr7;

/*! \brief constructor
 */
Gr7Ctrl::Gr7Ctrl(int robot_id, int nb_opp): Controller(GR7_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr7Ctrl::~Gr7Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr7Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr7Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr7Ctrl::close_controller()
{
	controller_finish(cvs);
}
