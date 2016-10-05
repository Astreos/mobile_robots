/*! 
 * \author Nicolas Van der Noot
 * \file cpp_interface_gestion.h
 * \brief C++ interface gestion to be called by C code
 */

#include "mbs_data.h"

#ifdef __cplusplus
extern "C" {
#endif
	void simu_init(MbsData* mbs_data);
	void simu_loop(MbsData* mbs_data);
	void simu_finish(MbsData* mbs_data);

	void simu_hJ_init(MbsData* mbs_data, double *h, double **Jac);
	void simu_jdqd_init(MbsData* mbs_data, double *jdqd);
	
	void simu_hJ_compute(MbsData* mbs_data, double *h, double **Jac);
	void simu_jdqd_compute(MbsData* mbs_data, double *jdqd);

	void rolling_integrate_pos(MbsData* mbs_data);
	void rolling_set_der(MbsData* mbs_data);

	void set_robot_init_pos(MbsData* mbs_data);

	void upate_position_contact(MbsData *mbs_data);
	void get_robot_force_torque(MbsData *mbs_data, int robot_index, double *Fx, double *Fy, double *Mz);
#ifdef __cplusplus
}
#endif
