
#ifndef _MBS_PATH_H
#define _MBS_PATH_H

#include "mbsysc_utilities_export.h"

MBSYSC_UTILITIES_EXPORT char* find_user_path(const char* mbsfile);
MBSYSC_UTILITIES_EXPORT void find_project_path(const char* mbsfile, char* prjpath);

#endif
