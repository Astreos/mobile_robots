#include "Gr14Ctrl.hh"
#include "ctrl_main_gr14.h"
#include <iostream>

using namespace ctrlGr14;

/*! \brief constructor
 */
Gr14Ctrl::Gr14Ctrl(int robot_id, int nb_opp): Controller(GR14_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr14Ctrl::~Gr14Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr14Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr14Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr14Ctrl::close_controller()
{
	controller_finish(cvs);
}
