
#include "ListElements.hh"
#include "ListPhysicHard.hh"
#include "ListPhysicConfig.hh"

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 */
ListElements::ListElements(MbsData *mbs_data)
{
	this->mbs_data = mbs_data;

	list_phys_elem = new ListPhysicConfig(mbs_data);

	list_targets = new ListTargets();

	list_bases = new ListBases();
}

/*! \brief destructor
 */
ListElements::~ListElements()
{
	delete list_bases;
	delete list_targets;
	delete list_phys_elem;
}
