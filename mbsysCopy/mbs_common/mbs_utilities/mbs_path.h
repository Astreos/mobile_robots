
#ifndef _MBS_PATH_H
#define _MBS_PATH_H

#include "mbsysc_utilities_export.h"

 /*! \brief Find the user folder corresponding to the given *.mbs file.
  *
  * This function allocat a char* that must be freed by the user.
  * 
  * \param[in] mbsfile the path and filename to the .mbs file
  *
  * \return pointer to char array containing the path to the userfctR folder.
  */ 
MBSYSC_UTILITIES_EXPORT char* find_user_path(const char* mbsfile);

/**
 * \brief Find the project directory (containing dataR, symbolicR, ...).
 * 
 * prjpath should be large enough to contain at least mbsfile.
 * 
 * \param[in] mbsfile the path and filename to the .mbs file
 * \param[out] prjpath the path to the root of the mbs project
 */
MBSYSC_UTILITIES_EXPORT void find_project_path(const char* mbsfile, char* prjpath);

#endif
