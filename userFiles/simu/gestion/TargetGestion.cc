#include "TargetGestion.hh"
#include "Robot.hh"

/*! \brief constructor
 * 
 * \param[in] list_elem list of elements
 */
TargetGestion::TargetGestion(ListElements *list_elem, MbsData *mbs_data): Gestion(list_elem)
{
	this->mbs_data = mbs_data;

	targets = list_elem->get_list_targets();

	list_phys_elem = list_elem->get_list_phys_elem();

	nb_robot = list_phys_elem->get_nb_robot();
}

/*! \brief destructor
 */
TargetGestion::~TargetGestion()
{
	
}

/*! \brief targets update
 */
void TargetGestion::compute()
{
	int target_seen;
	double target_x, target_y, target_z;
	double robot_x, robot_y;

	Robot *cur_robot;
	Target *cur_target;

	// loop on all the robots: detect targets
	for(int i=0; i<nb_robot; i++)
	{
		cur_robot = list_phys_elem->get_robot(i);

		robot_x = cur_robot->get_pose_x();
		robot_y = cur_robot->get_pose_y();

		// release all targets
		if (cur_robot->get_ctrl()->get_outputs()->flag_release)
		{
			cur_robot->release_all_targets();
		}
		else // grab and keep targets
		{
			target_seen = 0;

			for(int j=0; j<targets->get_nb_targets(); j++)
			{
				cur_target = targets->get_target(j);

				if (cur_target->get_state() == TARGET_GROUND) // current target on the ground, still possible to move
				{
					if (cur_target->point_inside(robot_x, robot_y))
					{
						if (!target_seen)
						{
							target_seen = 1;
							cur_robot->update_target_seen(cur_target);
						}
						break;
					}
				}
			}

			// no target seen
			if (!target_seen)
			{
				cur_robot->update_target_seen(NULL);
			}
		}
	}

	// loop on all the robots: move targets
	for(int i=0; i<nb_robot; i++)
	{
		cur_robot = list_phys_elem->get_robot(i);

		cur_robot->update_all_targets_position();
	}
}
