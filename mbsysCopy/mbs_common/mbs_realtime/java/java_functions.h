/*! 
 * \author Nicolas Van der Noot
 * \file java_functions.h
 * \brief Java functions used for the visualization
 */

#ifdef JAVA

#ifndef _JAVA_FUNCTIONS_H_
#define _JAVA_FUNCTIONS_H_

#include <jni.h>
#include "realtime.h"
#include "mbs_data.h"

#if defined(__APPLE__) || defined(__MACH__)
    #define APPLE_VISU
#endif

// -- Structures -- //

/*! \brief JNI (Java Native Interface) main structure
 */
typedef struct JNI_struct
{
    JNIEnv* env; ///< JNI environment
    jmethodID updateJointMethod; ///< update joint method
    jmethodID updateJtMultiMethod; ///< update joint method (multi-models)
    jobject obj; ///< object fot visualization

    jmethodID setVpMethod; ///< used to define the viewpoint
    jmethodID getNbVpMethod; ///< used to get the number of viewpoints

    jdoubleArray *doubleArrayArgs; ///< array to store the joints of the different models

} JNI_struct;

/*! \brief JNI input and output (the structure itself)
 */
typedef struct JNI_in_out
{
    // in
    int nb_models;
    int *nb_q; ///< input: number of joints
    int start_viewpoint; ///< input: first viewpoint
    double **q_vec;  ///< input: joint position
    char **mbs_file; ///< input: path to the visualization .mbs file

    // out
    JNI_struct *jni_struct; ///< output: JNI structure of the visualization
    int nb_viewpoint; ///< number of viewpoints

} JNI_in_out;

// functions prototypes

JNIEnv* create_vm(void);
void init_jni(Realtime_java *java, int nb_models, int *nb_q, double **q_vec, char **mbs_file, int start_viewpoint);

#ifdef APPLE_VISU
void run_java_mac_os(JNI_struct *jni_struct, int nb_models, int *nb_q);
void* start_jni(void *args);
#else
void start_jni(JNI_in_out *jni_in_out);
#endif

void update_jni(JNI_struct *jni_struct, Realtime_java *java, int nb_models, int *nb_q, double **q_vec);
void free_jni(JNI_struct *jni_struct);
void update_java(Simu_realtime *realtime);

#endif
#endif
