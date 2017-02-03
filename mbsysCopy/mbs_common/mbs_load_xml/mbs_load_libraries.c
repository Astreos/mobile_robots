#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// functions to dynamically load libraries


#include "mbs_load_libraries.h"
#include "mbs_path.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


MbsDataLibInfo* mbs_load_dynamic_library(char* libpath){


    MbsDataLibInfo* li = (MbsDataLibInfo*)malloc(sizeof(MbsDataLibInfo)) ;

	#ifdef UNIX
    char *error;

    // note : no need to call dlclose(), the library will be automatically closed at the end of the execution
    // ! ! ! ==> except for simulink since matlab process is still running ! ! !
    li->lib_handle = dlopen(libpath, RTLD_LAZY);
    if (!li->lib_handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    
    #else
	DWORD err;

	li->lib_handle = LoadLibrary(libpath);
	if(!li->lib_handle)
	{
		err = GetLastError();
		printf("DLL Failed To Load!\n error %d \n", err);
		//exit(1);
	}
	#endif

    return li;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

 /**
  * 
  * Close the library pointed by MbsDataLibInfo and free the memory
  * associated to the given MbsDataLibInfo.
  * 
  */ 

void mbs_close_dynamic_library(MbsDataLibInfo* li){

	#ifdef UNIX

    dlclose(li->lib_handle);

    #else

        // TO BE DONE

	#endif
    
    free(li);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef UNIX
void* mbs_load_function(MbsDataLibInfo* li, char* fct_name){

	char *error;
	void* fct;

    fct = dlsym(li->lib_handle, fct_name);

    if ((error = dlerror()) != NULL)
    {
          fprintf(stderr, "Error loading %s: %s\n", fct_name, error);
          exit(1);
    }

    return fct;
}
#else

FARPROC WINAPI mbs_load_function(MbsDataLibInfo* li, char* fct_name){

	FARPROC Proc;
	DWORD error;

	Proc = GetProcAddress(li->lib_handle, fct_name);

	if(Proc == NULL)
	{
		  error = GetLastError();
          fprintf(stderr, "Error loading %s: %d\n", fct_name, error);
          //exit(1);
    }
	else  return Proc;

}

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_load_symbolic_functions(MbsData* mbs_data, const char* mbs_filename, const char* build_name){

    char *lib_path;
	MbsDataLibInfo* symbLibInfo;

    // allocate space for the path to the user folder
    lib_path = (char*)malloc((strlen(mbs_filename)+strlen(build_name)+50)*sizeof(char));

    #ifdef UNIX
    sprintf(lib_path, "%s/symbolicR/libProject_symbolic.so", build_name);
    #else
	sprintf(lib_path, "%s\\Debug\\Project_symbolic.dll", build_name);
    #endif

    symbLibInfo = mbs_load_dynamic_library(lib_path) ;

    mbs_data->fct.symb.mbs_link      = (mbs_link_ptr)      mbs_load_function(symbLibInfo, "mbs_link");
	mbs_data->fct.symb.mbs_link3D    = (mbs_link3D_ptr)    mbs_load_function(symbLibInfo, "mbs_link3D");
    mbs_data->fct.symb.mbs_extforces = (mbs_extforces_ptr) mbs_load_function(symbLibInfo, "mbs_extforces");
    mbs_data->fct.symb.mbs_accelred  = (mbs_accelred_ptr)  mbs_load_function(symbLibInfo, "mbs_accelred");
    mbs_data->fct.symb.mbs_dirdyna   = (mbs_dirdyna_ptr)   mbs_load_function(symbLibInfo, "mbs_dirdyna");
    mbs_data->fct.symb.mbs_invdyna   = (mbs_invdyna_ptr)   mbs_load_function(symbLibInfo, "mbs_invdyna");
    mbs_data->fct.symb.mbs_cons_hJ   = (mbs_cons_hJ_ptr)   mbs_load_function(symbLibInfo, "mbs_cons_hJ");
    mbs_data->fct.symb.mbs_cons_jdqd = (mbs_cons_jdqd_ptr) mbs_load_function(symbLibInfo, "mbs_cons_jdqd");
    mbs_data->fct.symb.mbs_sensor    = (mbs_sensor_ptr)    mbs_load_function(symbLibInfo, "mbs_sensor");

    
    mbs_data->fct.symbolicLibHandle = symbLibInfo;

    free(lib_path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void mbs_load_user_functions(MbsData* mbs_data, const char* mbs_filename, const char* build_name){

    char *lib_path;
	MbsDataLibInfo* userLibInfo;

    // allocate space for the path to the user folder
    lib_path = (char*)malloc((strlen(mbs_filename)+strlen(build_name)+50)*sizeof(char));

    #ifdef UNIX
    sprintf(lib_path, "%s/userfctR/libProject_userfct.so", build_name);
    #else
	sprintf(lib_path, "%s\\Debug\\Project_userfct.dll", build_name);
    #endif

    userLibInfo = mbs_load_dynamic_library(lib_path) ;


    mbs_data->fct.user.user_JointForces   = (user_JointForces_ptr)   mbs_load_function(userLibInfo, "user_JointForces");
    mbs_data->fct.user.user_dirdyn_init   = (user_dirdyn_init_ptr)   mbs_load_function(userLibInfo, "user_dirdyn_init");
    mbs_data->fct.user.user_dirdyn_loop   = (user_dirdyn_loop_ptr)   mbs_load_function(userLibInfo, "user_dirdyn_loop");
    mbs_data->fct.user.user_dirdyn_finish = (user_dirdyn_finish_ptr) mbs_load_function(userLibInfo, "user_dirdyn_finish");
    mbs_data->fct.user.user_Derivative    = (user_Derivative_ptr)    mbs_load_function(userLibInfo, "user_Derivative");
    mbs_data->fct.user.user_DrivenJoints  = (user_DrivenJoints_ptr)  mbs_load_function(userLibInfo, "user_DrivenJoints");
    mbs_data->fct.user.user_cons_hJ       = (user_cons_hJ_ptr)       mbs_load_function(userLibInfo, "user_cons_hJ");
    mbs_data->fct.user.user_cons_jdqd     = (user_cons_jdqd_ptr)     mbs_load_function(userLibInfo, "user_cons_jdqd");
    mbs_data->fct.user.user_LinkForces    = (user_LinkForces_ptr)    mbs_load_function(userLibInfo, "user_LinkForces");
    mbs_data->fct.user.user_ExtForces     = (user_ExtForces_ptr)     mbs_load_function(userLibInfo, "user_ExtForces");

    mbs_data->fct.user.mbs_new_user_IO         = (mbs_new_user_IO_ptr)         mbs_load_function(userLibInfo, "mbs_new_user_IO");
    mbs_data->fct.user.mbs_new_user_model      = (mbs_new_user_model_ptr)      mbs_load_function(userLibInfo, "mbs_new_user_model");
    mbs_data->fct.user.mbs_delete_user_model   = (mbs_delete_user_model_ptr)   mbs_load_function(userLibInfo, "mbs_delete_user_model");
    mbs_data->fct.user.mbs_delete_user_IO      = (mbs_delete_user_IO_ptr)      mbs_load_function(userLibInfo, "mbs_delete_user_IO");
    mbs_data->fct.user.mbs_load_user_model_xml = (mbs_load_user_model_xml_ptr) mbs_load_function(userLibInfo, "mbs_load_user_model_xml");

    #ifdef REAL_TIME
        mbs_data->fct.user.user_realtime_options = (user_realtime_options_ptr) mbs_load_function(userLibInfo, "user_realtime_options");

        #ifdef SDL
            mbs_data->fct.user.user_keyboard         = (user_keyboard_ptr)         mbs_load_function(userLibInfo, "user_keyboard");
            mbs_data->fct.user.user_realtime_plot    = (user_realtime_plot_ptr)    mbs_load_function(userLibInfo, "user_realtime_plot");
            mbs_data->fct.user.user_joystick_axes    = (user_joystick_axes_ptr)    mbs_load_function(userLibInfo, "user_joystick_axes");
            mbs_data->fct.user.user_joystick_buttons = (user_joystick_buttons_ptr) mbs_load_function(userLibInfo, "user_joystick_buttons");
        #endif

        #ifdef VISU_3D
            mbs_data->fct.user.user_realtime_visu = (user_realtime_visu_ptr) mbs_load_function(userLibInfo, "user_realtime_visu");
        #endif
    #endif

    mbs_data->fct.userfctLibHandle = userLibInfo;

    free(lib_path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void mbs_close_libraries(MbsData* mbs_data){
    mbs_close_dynamic_library(mbs_data->fct.symbolicLibHandle);
    mbs_close_dynamic_library(mbs_data->fct.userfctLibHandle);
}

