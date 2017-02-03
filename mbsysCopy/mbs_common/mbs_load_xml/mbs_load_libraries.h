#ifndef _MBS_LOAD_LIBRARIES_H_
#define _MBS_LOAD_LIBRARIES_H_

#include "mbs_data.h"

void mbs_load_user_functions(MbsData* mbs_data, const char* mbs_filename, const char* build_name);
void mbs_load_symbolic_functions(MbsData* mbs_data, const char* mbs_filename, const char* build_name);

/**
 * Close the symbolic and userfct libraries associated to the given 
 * MbsData
 * 
 */ 
void mbs_close_libraries(MbsData* mbs_data);

#endif
