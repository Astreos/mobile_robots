
#include "GlobalGestion.hh"
#include "TimeGestion.hh"
#include "RobotGestion.hh"
#include "TargetGestion.hh"
#include "VisuGestion.hh"
#include "ScoreGestion.hh"

/*! \brief constructor
 */
GlobalGestion::GlobalGestion(MbsData *mbs_data, ListElements *list_elem, ActuatorGestion *actuators_gestion)
{
	this->mbs_data = mbs_data;

	this->list_elem = list_elem;

	pos_gestion = new PositionGestion(list_elem, mbs_data);

	contact_gestion = new ContactGestion(list_elem, mbs_data);

	for(int i=0; i<NB_GESTION; i++)
	{
		switch (i)
		{
			case TIME_GESTION:
				gestion_tab.push_back(new TimeGestion(list_elem, mbs_data));
				break;

			case ROBOT_GESTION:
				gestion_tab.push_back(new RobotGestion(list_elem, mbs_data, actuators_gestion));
				break;

			case TARGET_GESTION:
				gestion_tab.push_back(new TargetGestion(list_elem, mbs_data));
				break;

			case VISU_GESTION:
				gestion_tab.push_back(new VisuGestion(list_elem, mbs_data));
				break;

			case SCORE_GESTION:
				gestion_tab.push_back(new ScoreGestion(list_elem, mbs_data));
				break;

			default:
				std::cout << "Error: unknown GlobalGestion ID: " << i << " !" << std::endl;
				exit(EXIT_FAILURE);
		}
	}
}

/*! \brief destructor
 */
GlobalGestion::~GlobalGestion()
{
	for (unsigned int i=0; i<gestion_tab.size(); i++)
	{
		delete gestion_tab[i];	
	}

	delete contact_gestion;
	delete pos_gestion;
	delete list_elem;
}

/*! \brief main gestion function
 */
void GlobalGestion::compute()
{
	for (unsigned int i=0; i<gestion_tab.size(); i++)
	{
		gestion_tab[i]->compute();
	}
}
