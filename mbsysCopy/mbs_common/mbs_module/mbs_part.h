
#ifndef PART_h
#define PART_h

#include "mbs_xml_reader.h"
#include "mbs_data.h"
#include "MBSfun.h"

#include "mbsysc_module_export.h"


 /**
  * Structure defining the options of the coordinate
  * partitioning module
  *
  */
typedef struct MbsPartOptions
{
    int rowperm;       ///< 1 to allow line permutation; 0 otherwise, defaut = 0
    int visualise; // no = 0, yes = 1, defaut = 0
    double treshold;   ///< Maximal error on constraint allowed in Newton-Raphson algorithm, defaut = 1e-9
    int drivers; // no = 0, yes = 1, defaut = 0
    int verbose;       ///< 1 to get print indications related partitioning module; 0 otherwise, defaut = 1
    int clearmbsglobal; // inout = 1, out = 2, none = 3, all = 4, defaut = 1

} MbsPartOptions;


 /**
  * Main structure for the coordinate partitioning module.
  * This structure is allocated by the user and must be
  * passed to the mbs_run_part function.
  *
  */

typedef struct MbsPart
{
    MbsPartOptions *options; ///< Structure containing the options for coordinate partitioning module

    int n_qu;       ///< Number of independent variable needed
    int *ind_qu;    ///< Array with the indices of best choice for independent variables

    int n_qv;       ///< Number of dependent variable needed
    int *ind_qv;    ///< Array with the indices of best choice and order for dependent variables

    int n_hu;       ///< Number of independent constraint
    int *ind_hu;    ///< Array with the indices of best choice for independent constraints 

    int n_hv;       ///< Number of redundant constraint
    int *ind_hv;    ///< Array with the indices of best choice for redundant constraints 

    double *q_closed; ///< Array with the generalized coordinate in closed configuration (constraints solved)

} MbsPart;



#ifdef __cplusplus
extern "C" {
#endif
 /**
  * \brief Main function of the coordinate partitioning module.
  * It compute the coordinate partitioning for the given
  * MbsData structure.
  * 
  * @param mbs_part the coord. part. structure
  * @param mbs_data the structure of the mbs for which the coord.
  *       part. must be performed
  *
  */
MBSYSC_MODULE_EXPORT int mbs_run_part(MbsPart*  mbs_part, MbsData* mbs_data);

 /** 
  * \brief Allocate a new MbsPart structure for the given MbsData structure.
  * A new MbsPartOptions is also allocated and a pointer
  * to this structure is kept from the new MbsPart.
  *
  * \param mbs_data the structure of the mbs for which the coordinate part. will be performed.
  */
MBSYSC_MODULE_EXPORT MbsPart* mbs_new_part(MbsData* mbs_data);

 /**
  * \brief Free the memory of the given MbsPart structure.
  * The memory of the associated options (mbs_part->options)
  * is also freed.
  *
  * \param mbs_part the structure to be freed.
  */
MBSYSC_MODULE_EXPORT void mbs_delete_part(MbsPart* mbs_part);

 /**
  * \brief Allocate a new MbsPartOptions structure and intialize
  options with default values.
  */
MbsPartOptions* mbs_new_part_options(void);

 /**
  * \brief Free the memory of the given MbsPartOptions structure.
  * This function will be called by mbs_delete_part() and should never be called by the user.
  *
  * \param part_option_strct the structure to be freed.
  */
void mbs_delete_part_options(MbsPartOptions* part_option_strct);


/**
 * Utility function
 */
#ifdef __cplusplus
}
#endif
//void PART_get_options_from_user(MbsPartOptions *options);

#endif
