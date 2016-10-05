
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
    int rowperm; // no = 0, yes = 1, defaut = 0
    int visualise; // no = 0, yes = 1, defaut = 0
    double treshold; // defaut = 1e-9
    int drivers; // no = 0, yes = 1, defaut = 0
    int verbose; // no = 0, yes = 1, defaut = 1
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
    MbsPartOptions *options; 

    int n_qu; 
    int *ind_qu;

    int n_qv; 
    int *ind_qv; 

    int n_hu; 
    int *ind_hu; 

    int n_hv; 
    int *ind_hv; 

    double *q_closed;

} MbsPart;

 /**
  * Main function of the coordinate partitioning module.
  * It compute the coordinate partitioning for the given
  * MbsData structure.
  * 
  * @param mbs_part the coord. part. structure
  * @param mbs_data the structure of the mbs for which the coord.
  *       part. must be performed
  *
  */

#ifdef __cplusplus
extern "C" {
#endif

MBSYSC_MODULE_EXPORT int mbs_run_part(MbsPart*  mbs_part, MbsData* mbs_data);

 /** 
  * Allocate a new MbsPart structure.
  * A new MbsPartOptions is also allocated and a pointer
  * to this structure is kept from the new MbsPart.
  */
MBSYSC_MODULE_EXPORT MbsPart* mbs_new_part(MbsData* mbs_data);

 /**
  * Free the memory of the given MbsPart structure.
  * Th memory of the associated options (mbs_part->options)
  * is also freed.
  */
MBSYSC_MODULE_EXPORT void mbs_delete_part(MbsPart* mbs_part);

 /**
  * Allocate a new MbsPartOptions structure and intialize
  * options with default values.
  */
MbsPartOptions* mbs_new_part_options(void);

 /**
  * Free the memory of the given MbsPartOptions structure.
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
