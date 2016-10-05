
#ifndef MBS_XML_READER_h
#define MBS_XML_READER_h

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Init and free functions for MDS structures  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//typedef struct MDS_d_data_strct; 

typedef struct MDS_d_data_strct
{
    int d_type; // other = 0; simplified = 1, forced = 2, optimized = 3, resulting = 4, related = 5,
    double d_0; // initial value for optimization
    double d_min; // bounds values for optimization
    double d_max;
    double d_opti; // result value of optimization

    // the following variables are "comptute" in "MDS_opti_structurer"
    int is_related; // unrelated = 0, master = 1, equal salve = 2, opposite slave = 3
    struct MDS_d_data_strct *master_d_ptr;

    double *d_ptr;
    // link in the creation of mbs_data
    double *MBSdata_d_ptr;

} MDS_d_data_strct;

typedef struct MDS_ref_point_strct
{
    char *bodyname; 
    char *pointname;

} MDS_ref_point_strct;

typedef struct MDS_sensor_strct
{
    char *name; 

    int Pos; 
    int Rot; 
    int Vit; 
    int Om; 
    int Acc; 
    int Omp; 
    int Jac; 

} MDS_sensor_strct;

typedef struct MDS_extforce_strct
{
    char *name; 

    int Pos; 
    int Rot; 
    int Vit; 
    int Om; 
    int Acc; 
    int Omp; 

} MDS_extforce_strct;


typedef struct MDS_point_strct
{
    char *name;
    double *pt; // x y z

    MDS_sensor_strct *sensor;
    MDS_extforce_strct *extforce;

    MDS_d_data_strct **d_pt; // x y z
    int is_symmmetric; // asymmetric = 0, symmetric master = 1, symmetric salve = 2
    char *symmetric_point_name; // the name of the symmetric point (filed if master, NULL if slave or non symetric)

} MDS_point_strct;

typedef struct MDS_base_strct
{
    double *gravity; //x y z
    int n_base_point;
    MDS_point_strct **base_point_list;

} MDS_base_strct;

typedef struct MDS_joint_strct
{
    char *name;
    int type;  // T1=1, T2=2, T3=3, R1=4, R2=5, R3=6
    int nature; // independant=1, dependent=2, driven=3

    double q0;
    double qd0;
    double qdd0;

    int actuated;

    MDS_d_data_strct *d_qf; // d_data for the forced q
    int is_symmmetric; // asymmetric = 0, symmetric master = 1, symmetric salve = 2
    char *symmetric_joint_name; // the name of the symmetric joint (filed if master, NULL if slave or non symetric)


} MDS_joint_strct;

typedef struct MDS_body_strct
{
    char *name;

    MDS_ref_point_strct* parent;

    int n_joint;
    MDS_joint_strct **joint_list; 

    double mass; 
    double *com; //x y z
    double *inertia; // Ixx Ixy Ixz Iyy Iyz Izz

    int n_point; 
    MDS_point_strct **point_list; 

} MDS_body_strct;

typedef struct MDS_bodytree_strct
{
    int n_body;
    MDS_body_strct **body_list;

    int n_joint;
    MDS_joint_strct **joint_list;

    int n_qu;
    int *qu;

    int n_qv;
    int *qv;

    int n_qc;
    int *qc;

    int n_qlocked;
    int *qlocked;

    int n_qdriven; 
    int *qdriven;

    int n_qa;
    int *qa;    

} MDS_bodytree_strct;


typedef struct MDS_ball_strct
{
    char *name;  
 
    MDS_ref_point_strct *endpoint1; 
    MDS_ref_point_strct *endpoint2;

    int *ignore; // x y z 

} MDS_ball_strct;

typedef struct MDS_rod_strct
{
    char *name;  

    MDS_ref_point_strct *endpoint1; 
    MDS_ref_point_strct *endpoint2; 

    double length; 

    MDS_d_data_strct *d_length; // d_data for the length
    int is_symmmetric; // asymmetric = 0, symmetric master = 1, symmetric salve = 2
    char *symmetric_rod_name; // the name of the symmetric rod (filed if master, NULL if slave or non symetric)


} MDS_rod_strct;

typedef struct MDS_solid_strct
{
    char *name;  

    MDS_ref_point_strct *endpoint1; 
    MDS_ref_point_strct *endpoint2; 

    int *ignore; // x y z 

} MDS_solid_strct;

typedef struct MDS_cuts_strct
{
    int n_ball; 
    int n_rod; 
    int n_solid; 

    MDS_ball_strct **ball_list; 
    MDS_rod_strct **rod_list; 
    MDS_solid_strct **solid_list; 

} MDS_cuts_strct;

typedef struct MDS_link_strct
{
    char *name;

    MDS_ref_point_strct *endpoint1;
    MDS_ref_point_strct *endpoint2;

} MDS_link_strct;

typedef struct MDS_links_strct
{
    int n_link;
    MDS_link_strct **link_list;

    int n_link3D;
    MDS_link_strct **link3D_list;

} MDS_links_strct;

typedef struct MDS_parameter_strct
{
    char *name;
    int type; // scalar=1 ;  vector=2 ; lut1D=3 ; lut2D=4 ; state=5
    int n_value;
    double *value_list;

    MDS_d_data_strct **d_value; // d_data for the value
    int is_symmmetric; // asymmetric = 0, symmetric master = 1, symmetric salve = 2
    char *symmetric_parameter_name; // the name of the symmetric parameter (filed if master, NULL if slave or non symetric)

    /** if user model param is a structure name */
    char* structure_name;
    char* structure_header;
    /** if user model param is a lut1D or lut2D name */
    int   flag_PRJPATH;
    char* structure_lutfile;

} MDS_parameter_strct;

typedef struct MDS_user_model_strct
{
    char *name;
    int n_parameter; 
    MDS_parameter_strct **parameter_list; 
    
} MDS_user_model_strct;

typedef struct MDS_user_models_strct
{
    int n_user_model; 
    MDS_user_model_strct **user_model_list; 

} MDS_user_models_strct;

typedef struct MDS_user_IO_strct
{
    char *name;
    int type;   /** type=1: in ; type=2  */
    int size; 
    
} MDS_user_IO_strct;

typedef struct MDS_user_IOs_strct
{
    int n_user_IO; 
    int n_in,n_out;
    MDS_user_IO_strct **user_IO_list; 

} MDS_user_IOs_strct;

typedef struct MDS_opti_strct
{ 
    int n_d_simplified; 
    MDS_d_data_strct **d_simplified_list;

    int n_d_forced; 
    MDS_d_data_strct **d_forced_list;

    int n_d_optimized; 
    MDS_d_data_strct **d_optimized_list;

    int n_d_resulting; 
    MDS_d_data_strct **d_resulting_list;
    
    int n_d_related; 
    MDS_d_data_strct **d_related_list;

} MDS_opti_strct;

typedef struct MDS_gen_strct
{
    MDS_base_strct *base;
    MDS_bodytree_strct *bodytree;
    MDS_cuts_strct *cuts;
    MDS_links_strct *links;
    MDS_user_models_strct *user_models;
    MDS_user_IOs_strct *user_IOs;
    
    char *mbsfile;

    int n_point; 
    MDS_point_strct **point_list;

    int n_sensor; 
    int *sensor_point_list;

    int n_extforce; 
    int *extforce_point_list;

    int n_state;
    int n_state_value;
    MDS_parameter_strct **state_list;

    MDS_opti_strct *opti;

} MDS_gen_strct;


MDS_d_data_strct* init_MDS_d_data_strct(void);
void free_MDS_d_data_strct(MDS_d_data_strct *mds_d_data_strct);

MDS_ref_point_strct* init_MDS_ref_point_strct(void);
void free_MDS_ref_point_strct(MDS_ref_point_strct *mds_ref_point_strct);

MDS_sensor_strct* init_MDS_sensor_strct(char* name);
void free_MDS_sensor_strct(MDS_sensor_strct *mds_sensor_strct);

MDS_extforce_strct* init_MDS_extforce_strct(char* name);
void free_MDS_extforce_strct(MDS_extforce_strct *mds_extforce_strct);

MDS_point_strct* init_MDS_point_strct(char* name);
void free_MDS_point_strct(MDS_point_strct *mds_point_strct);

MDS_base_strct* init_MDS_base_strct(void);
void free_MDS_base_strct(MDS_base_strct *mds_base_strct);

MDS_joint_strct* init_MDS_joint_strct(char* name);
void free_MDS_joint_strct(MDS_joint_strct *mds_joint_strct);

MDS_body_strct* init_MDS_body_strct(char* name);
void free_MDS_body_strct(MDS_body_strct *mds_body_strct);

MDS_bodytree_strct* init_MDS_bodytree_strct(void);
void free_MDS_bodytree_strct(MDS_bodytree_strct *mds_bodytree_strct);

MDS_ball_strct* init_MDS_ball_strct(char* name);
void free_MDS_ball_strct(MDS_ball_strct *mds_ball_strct);

MDS_rod_strct* init_MDS_rod_strct(char* name);
void free_MDS_rod_strct(MDS_rod_strct *mds_rod_strct);

MDS_solid_strct* init_MDS_solid_strct(char* name);
void free_MDS_solid_strct(MDS_solid_strct *mds_solid_strct);

MDS_cuts_strct* init_MDS_cuts_strct();
void free_MDS_cuts_strct(MDS_cuts_strct *mds_cuts_strct);

MDS_link_strct* init_MDS_link_strct(char* name);
void free_MDS_link_strct(MDS_link_strct *mds_link_strct);

MDS_links_strct* init_MDS_links_strct();
void free_MDS_links_strct(MDS_links_strct *mds_links_strct);

MDS_parameter_strct* init_MDS_parameter_strct(char* name);
void free_MDS_parameter_strct(MDS_parameter_strct *mds_parameter_strct);

MDS_user_model_strct* init_MDS_user_model_strct(char* name);
void free_MDS_user_model_strct(MDS_user_model_strct *mds_user_model_strct);

MDS_user_models_strct* init_MDS_user_models_strct(void);
void free_MDS_user_models_strct(MDS_user_models_strct *mds_user_models_strct);

MDS_user_IO_strct* init_MDS_user_IO_strct(char* name);
void free_MDS_user_IO_strct(MDS_user_IO_strct *mds_user_IO_strct);

MDS_user_IOs_strct* init_MDS_user_IOs_strct(void);
void free_MDS_user_IOs_strct(MDS_user_IOs_strct *mds_user_IOs_strct);

MDS_opti_strct* init_MDS_opti_strct(void);
void free_MDS_opti_strct(MDS_opti_strct *mds_opti_strct);

MDS_gen_strct* init_MDS_gen_strct(void);
void free_MDS_gen_strct(MDS_gen_strct *mds_gen_strct);


MDS_gen_strct* MDS_mbs_reader(const char* mbs_xml_name);




#ifdef GEN_MDS
int MDS_write_mds_opti_xml(MDS_gen_strct *mds_gen_strct, const char *mds_opti_xml);
#else 
int MDS_update_mds_opti_xml(MDS_gen_strct *mds_gen_strct, const char *mds_opti_xml);
#endif

int MDS_read_mds_opti_xml(MDS_gen_strct *mds_gen_strct, const char *mds_opti_xml);

int MDS_opti_structurer(MDS_gen_strct* mds_gen_strct);
int MDS_opti_relate_data(MDS_gen_strct* mds_gen_strct);

int MDS_opti_slave_set_data(MDS_gen_strct* mds_gen_strct, double *optiNorms);
int MDS_opti_test_set_data(MDS_gen_strct* mds_gen_strct);

#endif




