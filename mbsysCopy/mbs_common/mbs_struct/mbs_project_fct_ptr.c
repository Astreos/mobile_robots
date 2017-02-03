 /**
  * 
  * Define template functions that call the user and symbolic function
  * stored via pointers in the MbsData structure
  * 
  * @date 23/06/2016
  * @author Nicolas Docquier
  * 
  * 
  */ 

#ifdef PRJ_FCT_PTR

#include "mbs_data.h"
#include "mbs_sensor.h"

//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//                       SYMBOLIC FUNCTIONS
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 



void mbs_link(double **frc,double **trq,double *Flnk,double *Z,double *Zd,MbsData *s, double tsim){
//	printf("mbs_project_fct_ptr - mbs_link \n");
    s->fct.symb.mbs_link(frc, trq, Flnk, Z, Zd, s, tsim);
}

void mbs_link3D(double **frc,double **trq, MbsData *s, double tsim){
//	printf("mbs_project_fct_ptr - mbs_link3D \n");
    s->fct.symb.mbs_link3D(frc, trq, s,tsim);
}

void mbs_extforces(double **frc, double **trq, MbsData *s, double tsim){
//	printf("mbs_project_fct_ptr - mbs_extforces \n");
    s->fct.symb.mbs_extforces(frc, trq, s, tsim);
}

int mbs_accelred(MbsData *s, double tsim){
//	printf("mbs_project_fct_ptr - mbs_accelred \n");
    return s->fct.symb.mbs_accelred(s, tsim);
}

void mbs_dirdyna(double **M,double *c, MbsData *s, double tsim){
//	printf("mbs_project_fct_ptr - mbs_dirdyna \n");
	s->fct.symb.mbs_dirdyna(M, c, s, tsim);    
}

void mbs_invdyna(double *Q, MbsData *s, double tsim){
//	printf("mbs_project_fct_ptr - mbs_dirdyna \n");
	s->fct.symb.mbs_invdyna(Q, s, tsim);    
}

void mbs_cons_hJ(double *h,double **Jac,MbsData *s, double tsim){
//	printf("mbs_project_fct_ptr - mbs_cons_hJ \n");
    s->fct.symb.mbs_cons_hJ(h, Jac, s, tsim);
}

void mbs_cons_jdqd(double *Jdqd,MbsData *s, double tsim){
//	printf("mbs_project_fct_ptr - mbs_cons_jdqd \n");
    s->fct.symb.mbs_cons_jdqd(Jdqd, s, tsim);
}

void  mbs_sensor(MbsSensor *sens, MbsData *s, int isens){
    s->fct.symb.mbs_sensor(sens, s, isens);
}            


//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//                       USER FUNCTIONS
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

double* user_JointForces(MbsData* s, double tsim){
//	printf("mbs_project_fct_ptr - user_JointForces \n");
    return s->fct.user.user_JointForces(s,tsim);
}

double user_LinkForces(double Z, double Zd, MbsData *s, double tsim, int ilnk){
    return s->fct.user.user_LinkForces(Z, Zd, s, tsim, ilnk);
}

double* user_ExtForces(double PxF[4], double RxF[4][4], 
                       double VxF[4], double OMxF[4], 
                       double AxF[4], double OMPxF[4], 
                       MbsData *s, double tsim,int ixF){
    return s->fct.user.user_ExtForces(PxF, RxF, VxF, OMxF, AxF, OMPxF, s, tsim, ixF);
}

void user_Derivative(MbsData *s){
    s->fct.user.user_Derivative(s);
}

void user_DrivenJoints(MbsData *s, double tsim){
    s->fct.user.user_DrivenJoints(s, tsim);
}

void user_cons_hJ(double *h, double **Jac, MbsData *s, double tsim){
	s->fct.user.user_cons_hJ(h, Jac, s, tsim);
}

void user_cons_jdqd(double *jdqd, MbsData *s, double tsim){
	s->fct.user.user_cons_jdqd(jdqd, s, tsim);
}


// User functions specific to dirdyn module

void user_dirdyn_init(MbsData *s, MbsDirdyn *mbs_dd){
	s->fct.user.user_dirdyn_init(s,mbs_dd);
}

void user_dirdyn_loop(MbsData *s, MbsDirdyn *mbs_dd){
	s->fct.user.user_dirdyn_loop(s, mbs_dd);
}

void user_dirdyn_finish(MbsData* s, MbsDirdyn *mbs_dd){
	s->fct.user.user_dirdyn_finish(s, mbs_dd);
}


/*
void mbs_delete_user_model(UserModel* ums){
	
}

void mbs_delete_user_IO(UserIO *uvs){
}

void mbs_load_user_model_xml(MDS_gen_strct* gen, UserModel* ums){
	
}
*/

#ifdef REAL_TIME

	
    void user_realtime_options(MbsData* s, Realtime_option *options){
		s->fct.user.user_realtime_options(s, options);
	}

	#ifdef SDL

		void user_keyboard(MbsData* s, Simu_realtime *r, int cur_t_usec, const Uint8 *keystates){
			s->fct.user.user_keyboard(s, r, cur_t_usec, keystates);
		}
		
		void user_realtime_plot(MbsData* s){
			s->fct.user.user_realtime_plot(s);
		}
		
		void user_joystick_axes(MbsData* s, Simu_realtime *r, int nb_joysticks){
			s->fct.user.user_joystick_axes(s, r, nb_joysticks);
		}
		
		void user_joystick_buttons(MbsData* s, int buttonID){
			s->fct.user.user_joystick_buttons(s, buttonID);
		}
	#endif

	#ifdef VISU_3D
        void user_realtime_visu(MbsData* s, int nb_models, int *nb_q, double **q_vec){
			s->fct.user.user_realtime_visu(s, nb_models, nb_q, q_vec);
		}
	#endif

#endif

#endif




