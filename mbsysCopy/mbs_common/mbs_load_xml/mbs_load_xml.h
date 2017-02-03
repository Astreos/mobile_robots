#ifndef mbs_load_xml_h
#define mbs_load_xml_h

#include "mbs_xml_reader.h"
#include "mbs_data.h"

#include "mbsysc_loadxml_export.h"

#ifdef __cplusplus
extern "C" {
#endif
 /**
  * \brief Load the data from the given *.mbs file (data in the xml 
           format). The memory of a new MbsData is allocated.
  * 
  * \param mbs_filename path to the file to load including filename and extension.
  * \param build_name path to the build folder of MBsysC libraries
  */
MBSYSC_LOADXML_EXPORT MbsData* mbs_load(const char* mbs_filename, const char* build_name);

 /**
  * \brief Free the memory used by the given MbsData structure.
  *
  * \param s the structure to be freed.
  */
MBSYSC_LOADXML_EXPORT void mbs_delete_data(MbsData *s);


void mbs_delete_data(MbsData *s);
#ifdef __cplusplus
}
#endif

 /**
  * \brief Load the user model data 
  */
PROJECT_USERFCT_EXPORT void mbs_load_user_model_xml(MDS_gen_strct* gen, UserModel* ums);

 /**
  * \brief Retrieve a MbsData structure from the given MDS_gen_strct
  *
  * \param[in] mds_gen_strct the MDS gen structure already filled to be used to fill the MbsData structure.
  * \param[in,out] s the MbsData structure that will be completed according to the MDS gen structure.
  */
MbsData* MDS_create_MBSdataStruct(MDS_gen_strct* mds_gen_strct, MbsData* s);


#endif
