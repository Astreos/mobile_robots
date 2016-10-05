#include "CppInterface.hh"

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 */
CppInterface::CppInterface(MbsData *mbs_data)
{
	ListElements *list_elem;

	this->mbs_data = mbs_data;

	rolling   = new RollingGestion(mbs_data);
	actuators = new ActuatorGestion(mbs_data);
	list_elem = new ListElements(mbs_data);
	global_gestion = new GlobalGestion(mbs_data, list_elem, actuators);
}

/*! \brief CppInterface desctructor
 */
CppInterface::~CppInterface()
{
	delete rolling;
	delete actuators;
	delete global_gestion;
}

/*! \brief compute the C++ modules
 */
void CppInterface::compute()
{
	actuators->compute();

	global_gestion->compute();
}
