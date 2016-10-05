/**
 *
 *   Universite catholique de Louvain
 *   CEREM : Centre for research in mechatronics
 *   http://www.robotran.be  
 *   Contact : info@robotran.be
 *
 *
 *   MBsysC main script template for simple model:
 *   -----------------------------------------------
 *    This template loads the data file *.mbs and execute:
 *      - the coordinate partitioning module
 *      - the direct dynamic module (time integration of
 *        equations of motion).
 *    It may be adapted and completed by the user.
 * 
 *    (c) Universite catholique de Louvain
 */

#include <stdio.h>
#include "mbs_part.h"
#include "mbs_load_xml.h"
#include "mbs_dirdyn.h"
#include "mbs_data.h"
#include "realtime.h"
#include "cmake_config.h"

#include "cpp_interface_gestion.h"

// for seed
#ifdef UNIX
	#include <sys/time.h>
#else
	#include <sys/timeb.h>
#endif

#ifdef REAL_TIME
extern "C" {
	#include "time_functions.h"
}
#elif defined(WIN32)
typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;

int gettimeofday (struct timeval *tp, void *tz)
{
	struct _timeb timebuffer;
	_ftime (&timebuffer);
	tp->tv_sec  = timebuffer.time;
	tp->tv_usec = timebuffer.millitm * 1000;
	return 0;
}
#endif

/*! \brief set the number of user constraints and update the related structures
 * 
 * \param[in,out] mbs_data Robotran structure
 * \param[in] Nuserc number of user constraints
 */
void set_Nuserc(MbsData *mbs_data, int Nuserc)
{
	int i;

	mbs_data->Nuserc = Nuserc;
	mbs_data->Ncons += Nuserc;
	mbs_data->nhu   += Nuserc;

	// re-init hu vector
	if(mbs_data->hu != NULL)
	{
		free(mbs_data->hu);
	}
	mbs_data->hu = (int*) calloc(mbs_data->nhu+1,sizeof(int));
	mbs_data->hu[0] = mbs_data->nhu;
	for (i=1; i<=mbs_data->nhu; i++)
	{
		mbs_data->hu[i] = i;
	}
}

int main(int argc, char const *argv[])
{
	MbsData *mbs_data;
	
	MbsPart *mbs_part;
	MbsDirdyn *mbs_dirdyn;

	struct timeval seed_time;


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	/*                   RANDOM SEED                             *
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	gettimeofday(&seed_time, NULL);
	srand(seed_time.tv_usec * seed_time.tv_sec);


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	/*                     LOADING                               *
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	printf("Loading the Eurobot project template for the robotics course !\n");
	mbs_data = mbs_load(PROJECT_SOURCE_DIR"/../dataR/m454_project.mbs", BUILD_PATH);

	// user constraints
	set_Nuserc(mbs_data, 12);


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	/*              COORDINATE PARTITIONING                      *
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	mbs_part = mbs_new_part(mbs_data);
	mbs_part->options->rowperm = 1;
	mbs_part->options->verbose = 1;
	mbs_run_part(mbs_part, mbs_data);
	
	mbs_delete_part(mbs_part);


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	/*                   DIRECT DYNANMICS                        *
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	mbs_dirdyn = mbs_new_dirdyn(mbs_data);

	// dirdyn options (see documentations for additional options)
	mbs_dirdyn->options->dt0 = 1e-3;
	mbs_dirdyn->options->tf  = 130.0;
	mbs_dirdyn->options->save2file = 0;
	mbs_dirdyn->options->respath  = PROJECT_SOURCE_DIR"/../resultsR";
	mbs_dirdyn->options->animpath = PROJECT_SOURCE_DIR"/../animationR";
	mbs_dirdyn->options->realtime = 1;

	// special debug parameters
	#ifdef DEBUG_VERSION
	mbs_dirdyn->options->realtime = 0;
	mbs_dirdyn->options->tf       = 5.0;
	#endif

	set_robot_init_pos(mbs_data);

	mbs_run_dirdyn(mbs_dirdyn, mbs_data);
	
	mbs_delete_dirdyn(mbs_dirdyn, mbs_data);


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	/*                   CLOSING OPERATIONS                      *
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	mbs_delete_data(mbs_data);
	
	return 0;
}
