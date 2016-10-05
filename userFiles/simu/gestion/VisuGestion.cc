#include "VisuGestion.hh"
#include "MovingObject.hh"
#include "user_all_id.h"
#include "config_functions.hh"
#include "config_file.h"

/*! \brief constructor
 * 
 * \param[in] list_elem list of elements
 */
VisuGestion::VisuGestion(ListElements *list_elem, MbsData *mbs_data): Gestion(list_elem)
{
	FILE *config_file;
	int nb_joints;

	config_file = open_config();

	this->mbs_data = mbs_data;

	list_phys_elem = list_elem->get_list_phys_elem();

	list_targets = list_elem->get_list_targets();

	#ifdef HARDCODED_CONFIG
	nb_joints = NB_JOINTS_ANIM;
	#else
	nb_joints = config_int(config_file, "nb_joints_anim");
	#endif

	for(int i=0; i<nb_joints; i++)
	{
		q_vector.push_back(0.0);
	}

	// main mbs files IDs
	main_mbs_id.push_back(FJ_T1_robot_B_id);
	main_mbs_id.push_back(FJ_T2_robot_B_id);
	main_mbs_id.push_back(FJ_R3_robot_B_id);
	main_mbs_id.push_back(Right_wheel_joint_B_id);
	main_mbs_id.push_back(Left_wheel_joint_B_id);
	main_mbs_id.push_back(Tower_joint_B_id);

	main_mbs_id.push_back(FJ_T1_robot_R_id);
	main_mbs_id.push_back(FJ_T2_robot_R_id);
	main_mbs_id.push_back(FJ_R3_robot_R_id);
	main_mbs_id.push_back(Right_wheel_joint_R_id);
	main_mbs_id.push_back(Left_wheel_joint_R_id);
	main_mbs_id.push_back(Tower_joint_R_id);

	main_mbs_id.push_back(FJ_T1_robot_Y_id);
	main_mbs_id.push_back(FJ_T2_robot_Y_id);
	main_mbs_id.push_back(FJ_R3_robot_Y_id);
	main_mbs_id.push_back(Right_wheel_joint_Y_id);
	main_mbs_id.push_back(Left_wheel_joint_Y_id);
	main_mbs_id.push_back(Tower_joint_Y_id);

	main_mbs_id.push_back(FJ_T1_robot_W_id);
	main_mbs_id.push_back(FJ_T2_robot_W_id);
	main_mbs_id.push_back(FJ_R3_robot_W_id);
	main_mbs_id.push_back(Right_wheel_joint_W_id);
	main_mbs_id.push_back(Left_wheel_joint_W_id);
	main_mbs_id.push_back(Tower_joint_W_id);

	for(unsigned int i=0; i<main_mbs_id.size(); i++)
	{
		main_mbs_anim_id.push_back(main_mbs_id[i]-1);
	}

	close_config(config_file);
}

/*! \brief destructor
 */
VisuGestion::~VisuGestion()
{

}

/*! \brief computation
 */
void VisuGestion::compute()
{
	main_mbs();

	moving_obj();

	targets();
}

/*! \brief update the joints related to the main mbs file
 */
void VisuGestion::main_mbs()
{
	for(unsigned int i=0; i<main_mbs_id.size(); i++)
	{
		q_vector[main_mbs_anim_id[i]] = mbs_data->q[main_mbs_id[i]];
	}
}

/*! \brief update the joints of the moving physical objects not in the main .mbs file
 */
void VisuGestion::moving_obj()
{
	int cur_T1, cur_T2, cur_R3;
	MovingObject *cur_moving;

	for(int i=0; i<list_phys_elem->get_nb_moving(); i++)
	{
		cur_moving = list_phys_elem->get_moving(i);

		list_phys_elem->fill_T1_T2_R3(i, cur_T1, cur_T2, cur_R3);

		q_vector[cur_T1-1] = cur_moving->get_pose_x();
		q_vector[cur_T2-1] = cur_moving->get_pose_y();
		q_vector[cur_R3-1] = cur_moving->get_alpha();
	}
}

/*! \brief update the joints of the targets
 */
void VisuGestion::targets()
{
	int cur_T1, cur_T2, cur_T3;
	double cur_x, cur_y, cur_z;

	for(int i=0; i<list_targets->get_nb_targets(); i++)
	{
		list_targets->fill_T1_T2_T3(i, cur_T1, cur_T2, cur_T3);

		list_targets->get_pos(i, cur_x, cur_y, cur_z);

		q_vector[cur_T1-1] = cur_x;
		q_vector[cur_T2-1] = cur_y;
		q_vector[cur_T3-1] = cur_z;
	}
}

/*! \brief fill the vector 'q_vec' of 'user_realtime_visu.cc'
 *
 * \param[in] nb_q number of joints to fill
 * \param[out] q_vec vector of 'user_realtime_visu.cc'
 */
void VisuGestion::fill_q_vec(int *nb_q, double **q_vec)
{
	// safety
	if (nb_q[0] != q_vector.size())
	{
		std::cout << "Error: nb_q (" << nb_q[0] << ") != q_vector.size() (" << q_vector.size() << ") !" << std::endl;
		exit(EXIT_FAILURE);
	}

	// fill 'q_vec'
	for(int i=0; i<nb_q[0]; i++)
	{
		q_vec[0][i] = q_vector[i];
	}
}
