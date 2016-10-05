
#ifdef JAVA

#include "java_functions.h"
#include "cmake_java_config.h"
#include "user_realtime.h"

#include <locale.h>


#include <stdlib.h>

#ifdef APPLE_VISU
    #include <unistd.h>
    #include <CoreFoundation/CoreFoundation.h>
    #include <pthread.h>

    pid_t pid_fork = -1; ///< PID of the fork

    int pipe_ready[2];        ///< descriptor pipe: flag after visu initialization
    int pipe_finish[2];       ///< descriptor pipe: flag to close the visualization
    int pipe_close[2];        ///< descriptor pipe: flag to tell that the visualization release is done
    int pipe_q_vec[2];        ///< descriptor pipe: position of the joints
    int pipe_viewpoint[2];    ///< descriptor pipe: viewpoint flag
    int pipe_nb_viewpoint[2]; ///< descriptor pipe: number of viewpoints
#endif

// version of the JDK
#define JNI_VERSION JNI_VERSION_1_6

#define START_VIEWPOINT 0 ///< default initial viewpoint



/*! \brief instantiate a java virtual machine so as to access to the 3D animation facilities of MBSysPad
 * 
 * \return JNI environment
 */
JNIEnv* create_vm()
{
    int res;

    JavaVM* jvm;
    JNIEnv* env;
    JavaVMInitArgs args;
    JavaVMOption options[2];
    
    // the following parameter should be adapted depending on the version of the JDK
    args.version = JNI_VERSION;
    // number of argument to pass to the jvm
    args.nOptions = 2;
    // path to the jar file containing mbsyspad bynary code
    options[0].optionString = "-Djava.class.path="JAR_PATH;
    // path to the folder containing additional libraries, in particular the java3D native file (libj3dcore-ogl.so for linux)
    options[1].optionString = "-Djava.library.path="J3D_PATH;

    args.options = options;
    args.ignoreUnrecognized = JNI_FALSE;

    res = JNI_CreateJavaVM(&jvm, (void **)&env, &args);

    if (res < 0)
    {
        switch ( res )
        {
            case -1:
                fprintf(stderr, "/* unknown error */\n");
                break;
            case -2:
                fprintf(stderr, "/* thread detached from the VM */\n");
                break;
            case -3:
                fprintf(stderr, "/* JNI version error */\n");
                break;
            case -4:
                fprintf(stderr, "/* not enough memory */\n");
                break;
            case -5:
                fprintf(stderr, "/* VM already created */\n");
                break;
            case -6:
                fprintf(stderr, "/* invalid arguments */\n");
                break;
            default:
                fprintf(stderr, "Can't create Java VM\n");
        }
        exit(1);
    }

    // reset to local to "C" to ensure the decimal separator to be a dot and not a coma
    setlocale(LC_ALL, "C");

    return env;
}

/*! \brief initialize the JNI structure as a void* function
 * 
 * \param[in,out] jni_in_out (or args) inputs and output of the JNI structure
 */
#ifdef APPLE_VISU
void* start_jni(void *args)
#else
void start_jni(JNI_in_out *jni_in_out)
#endif
{
    int i;
    #ifdef APPLE_VISU
    int flag_ready, nb_viewpoint;
    JNI_in_out *jni_in_out;
    #endif

    // - - - - - - - - - - - - - - - - -
    // declaration for 3D view with JNI

    jclass animFrameClass,  PadModelClass, MbsModelJ3DClass;
    jmethodID constructor;
    jmethodID loadMethod, setLocMethod;
    jmethodID getNbVpMethod, setVpMethod, setVisibleMethod;
    jobject model, model3D;
    jstring mbsFilename;
    jdoubleArray doubleArrayArg;

    JNIEnv* env;
    jmethodID updateJointMethod, updateJtMultiMethod;
    jobject obj;

    // - - - - - - - - - - - - - - - - - - - - - - -
    // initialise jni variable for 3D visualisation

    #ifdef APPLE_VISU
    jni_in_out = (JNI_in_out*) args;
    #endif

    env = create_vm();

    animFrameClass = (*env)->FindClass(env, "be/robotran/test/LiveSimAnimFrameCL");

    // get the reference to the constructor of this class
    constructor = (*env)->GetMethodID(env, animFrameClass, "<init>", "()V");

    // get the reference to the load method which load the *.mbs file
    loadMethod = (*env)->GetMethodID(env, animFrameClass, "load", "(Ljava/lang/String;)V");

    // get the reference to the updateJoints method which update the 3D view during simulation
    updateJointMethod = (*env)->GetMethodID(env, animFrameClass, "updateJoints", "([D)V");

    // get the reference to the updateJoints method which update the 3D view during simulation (multi-models)
    updateJtMultiMethod = (*env)->GetMethodID(env, animFrameClass, "updateJoints", "(I[D)V");

    // get the reference to the setLocation method
    setLocMethod = (*env)->GetMethodID(env, animFrameClass, "setLocation", "(II)V");

    // get the reference to the setViewpoint method
    setVpMethod = (*env)->GetMethodID(env, animFrameClass, "setViewpoint", "(I)V");

    // set visible to true
    setVisibleMethod = (*env)->GetMethodID(env, animFrameClass, "setVisible", "(Z)V");

    // get the reference to the setViewpoint method
    getNbVpMethod = (*env)->GetMethodID(env, animFrameClass, "getNbViewpoint", "()I");

    // create an instance of the 3D view
    obj = (*env)->NewObject(env, animFrameClass, constructor);

    // move the frame
    (*env)->CallObjectMethod(env, obj, setLocMethod, 100, 100);

    // load the mbsfilename
    for(i=0; i<jni_in_out->nb_models; i++)
    {
        (*env)->CallObjectMethod(env, obj, loadMethod, (*env)->NewStringUTF(env, jni_in_out->mbs_file[i]));
    }

    (*env)->CallObjectMethod(env, obj, setVpMethod, -1);

    jni_in_out->jni_struct->doubleArrayArgs = (jdoubleArray*) malloc(jni_in_out->nb_models*sizeof(jdoubleArray));

    for(i=0; i<jni_in_out->nb_models; i++)
    {
        // instantiate jni arrays of double
        jni_in_out->jni_struct->doubleArrayArgs[i] = (*env)->NewDoubleArray(env, jni_in_out->nb_q[i]);

        // copy the current values of q_vec to the jni array
        (*env)->SetDoubleArrayRegion(env, jni_in_out->jni_struct->doubleArrayArgs[i], 0, jni_in_out->nb_q[i], jni_in_out->q_vec[i]);

        // update the 3D view
        (*env)->CallObjectMethod(env, obj, updateJtMultiMethod, i, jni_in_out->jni_struct->doubleArrayArgs[i]);
    }

    // adapt the viewpoint
    (*env)->CallObjectMethod(env, obj, setVpMethod, jni_in_out->start_viewpoint);

    // set visible
    (*env)->CallObjectMethod(env, obj, setVisibleMethod, 1);

    // number of viewpoints
    jni_in_out->nb_viewpoint = (*env)->CallIntMethod(env, obj, getNbVpMethod);

    // JNI structure
    jni_in_out->jni_struct->env                 = env;
    jni_in_out->jni_struct->updateJointMethod   = updateJointMethod;
    jni_in_out->jni_struct->updateJtMultiMethod = updateJtMultiMethod;
    jni_in_out->jni_struct->obj                 = obj;
    jni_in_out->jni_struct->setVpMethod         = setVpMethod;
    jni_in_out->jni_struct->getNbVpMethod       = getNbVpMethod;

    #ifdef APPLE_VISU

    flag_ready = 1;
    write(pipe_ready[1], &flag_ready, sizeof(int));

    nb_viewpoint = jni_in_out->nb_viewpoint;
    write(pipe_nb_viewpoint[1], &nb_viewpoint, sizeof(int));

    run_java_mac_os(jni_in_out->jni_struct, jni_in_out->nb_models, jni_in_out->nb_q);

    return (0);
    #endif
}

/*! \brief initialize the JNI structure
 *
 * \param[out] java realtime Java structure
 * \param[in] nb_models number of models
 * \param[in] nb_q number of joints in the .mbs file
 * \param[in] q_vec vector of size nb_q with the initial values for q
 * \param[in] mbs_file path and name of the .mbs file used for visualization
 */
void init_jni(Realtime_java *java, int nb_models, int *nb_q, double **q_vec, char **mbs_file, int start_viewpoint)
{
    JNI_in_out jni_in_out;

    #ifdef APPLE_VISU
    pthread_t thr;
    int flag_ready, nb_viewpoint;
    #endif

    // inputs
    jni_in_out.nb_models = nb_models;
    jni_in_out.nb_q = nb_q;
    jni_in_out.start_viewpoint = start_viewpoint;
    jni_in_out.q_vec = q_vec;
    jni_in_out.mbs_file = mbs_file;

    // safety
    if (nb_models < 1)
    {
        printf("Error: nb_models for Java anim should be positive (not %d) !\n", nb_models);
        exit(EXIT_FAILURE);
    }

    // output
    jni_in_out.jni_struct = (JNI_struct*) malloc(sizeof(JNI_struct));

    #ifdef APPLE_VISU

    if ((pipe(pipe_ready) != 0) || (pipe(pipe_finish) != 0) || (pipe(pipe_close) != 0)
        || (pipe(pipe_q_vec) != 0) || (pipe(pipe_viewpoint) != 0) || (pipe(pipe_nb_viewpoint) != 0))
    {
        fprintf(stderr, "Error while creating the Java pipes.\n");
        exit(EXIT_FAILURE);
    }

    // fork: create two processes
    pid_fork = fork();

    if(pid_fork == -1)    
    {
        fprintf(stderr, "Error while creating the Java process (fork).\n");
        exit(EXIT_FAILURE);
    }

    if (!pid_fork) // son, in charge of the visualization
    {
        // pipes safety
        close(pipe_ready[0]);
        close(pipe_close[0]);
        close(pipe_nb_viewpoint[0]);
        close(pipe_finish[1]);
        close(pipe_q_vec[1]);
        close(pipe_viewpoint[1]);

        // save JNI adress
        java->jni_struct = jni_in_out.jni_struct;

        // JNI run
        if (pthread_create(&thr, NULL, start_jni, &jni_in_out))
        {
            printf("Error: JNI thread not properly created !\n");
            exit(EXIT_FAILURE);
        }
        CFRunLoopRun(); // Apple processes dispatch control
    }
    else // father, in charge of all simulation, except visualization
    {
        // pipes safety
        close(pipe_ready[1]);
        close(pipe_close[1]);
        close(pipe_nb_viewpoint[1]);
        close(pipe_finish[0]);
        close(pipe_q_vec[0]);
        close(pipe_viewpoint[0]);

        read(pipe_ready[0], &flag_ready, sizeof(int));

        read(pipe_nb_viewpoint[0], &nb_viewpoint, sizeof(int));
        java->nb_viewpoint = nb_viewpoint;
    }

    #else
    // JNI initialization
    start_jni(&jni_in_out);

    java->nb_viewpoint = jni_in_out.nb_viewpoint;

    // save JNI adress
    java->jni_struct = jni_in_out.jni_struct;
    #endif
}

/*! \brief Release memory
 *
 * \param[out] jni_struct JNI structure to release
 */
void free_jni(JNI_struct *jni_struct)
{
    #ifdef APPLE_VISU
    int flag_finish, flag_close;

    if(pid_fork) // father, in charge of all simulation, except visualization
    {
        flag_finish = 1;
        write(pipe_finish[1], &flag_finish, sizeof(int));
        read(pipe_close[0], &flag_close, sizeof(int));
    }
    #else
        free(jni_struct->doubleArrayArgs);
        free(jni_struct);
    #endif
}

/*! \brief update the Java visualization
 *
 * \param[in,out] realtime real-time structure
 */
void update_java(Simu_realtime *realtime)
{
    Realtime_java *java;
    MbsData* mbs_data;

    #ifdef APPLE_VISU
    int flag_finish;
    #endif

    java     = realtime->ext->java;
    mbs_data = realtime->ext->mbs_data;

    #ifdef APPLE_VISU
    if(pid_fork) // father, in charge of all simulation, except visualization
    {
        flag_finish = 0;
        write(pipe_finish[1], &flag_finish, sizeof(int));
    }
    else // son, in charge of the visualization
    {
        return; // safety, should never happen
    }
    #endif

    user_realtime_visu(mbs_data, java->nb_models, java->nb_q, java->cur_q);

    if (java->visu_past_flag)
    {
        java->visu_past_flag   = 0;
        java->last_past_q_flag = 1;

        update_jni(java->jni_struct, java, java->nb_models, java->nb_q, java->past_q);
    }
    else if (java->change_viewpoint)
    {
        if (java->last_past_q_flag)
        {
            update_jni(java->jni_struct, java, java->nb_models, java->nb_q, java->past_q);
        }
        else
        {
            update_jni(java->jni_struct, java, java->nb_models, java->nb_q, java->cur_q);
        }
    }
    else
    {
        java->last_past_q_flag = 0;

        update_jni(java->jni_struct, java, java->nb_models, java->nb_q, java->cur_q);
    }
}

/*! \brief update the 3D view with JNI
 *
 * \param[in] jni_struct JNI structure
 * \param[in] java Java real-time structure
 * \param[in] nb_models number of models
 * \param[in] nb_q number of joints to update for the visualization
 * \param[in] q_vec vector of size nb_q with the current values to plot
 */
void update_jni(JNI_struct *jni_struct, Realtime_java *java, int nb_models, int *nb_q, double **q_vec)
{
    int i;

    // variables declaration
    #ifdef APPLE_VISU
        int flag_viewpoint;
    #else
        JNIEnv* env;
    #endif

    #ifdef APPLE_VISU
        for(i=0; i<nb_models; i++)
        {
            // send joints position to visualization
            write(pipe_q_vec[1], q_vec[i], nb_q[i]*sizeof(double));
        }
    #else
        env = jni_struct->env;
    #endif

    // adapt the viewpoint
    if (java->change_viewpoint)
    {
        java->change_viewpoint = 0;

        java->cur_viewpoint++;

        if (java->cur_viewpoint >= java->nb_viewpoint)
        {
            java->cur_viewpoint = START_VIEWPOINT;
        }

        #ifdef APPLE_VISU
            flag_viewpoint = java->cur_viewpoint;
        #else
            (*env)->CallObjectMethod(env, jni_struct->obj, jni_struct->setVpMethod, java->cur_viewpoint);
        #endif
    }
    #ifdef APPLE_VISU
    else
    {
        flag_viewpoint = -1;
    }

    write(pipe_viewpoint[1], &flag_viewpoint, sizeof(int));
    #endif

    jint av;

    av = 0;

    // update the 3D view
    #ifndef APPLE_VISU
        for(i=0; i<nb_models; i++)
        {
            // copy the current values of q_vec to the jni array
            (*env)->SetDoubleArrayRegion(env, jni_struct->doubleArrayArgs[i], 0, nb_q[i], q_vec[i]);

            (*env)->CallObjectMethod(env, jni_struct->obj, jni_struct->updateJtMultiMethod, i, jni_struct->doubleArrayArgs[i]);
        }
    #endif
}

#ifdef APPLE_VISU
/*! \brief main loop of the visualization on Mac OS
 *
 * \param[in,out] jni_struct JNI structure
 * \param[in] nb_models number of models
 * \param[in] nb_q number of joints for each model
 */
void run_java_mac_os(JNI_struct *jni_struct, int nb_models, int *nb_q)
{
    // variables declaration
    int i, flag_finish, flag_close, flag_viewpoint;
    double **q_vec;
    JNIEnv* env;

    // JNI environment
    env = jni_struct->env;

    // vectors to get joint position
    q_vec = (double**) malloc(nb_models*sizeof(double*));

    for(int i=0; i<nb_models; i++)
    {
        q_vec[i] = (double*) malloc(nb_q[i]*sizeof(double));
    }

    while (1)
    {
        read(pipe_finish[0], &flag_finish, sizeof(int));

        if (flag_finish)
        {
            flag_close = 1;

            for(i=0; i<nb_models; i++)
            {
                free(q_vec[i]);
            }
            free(q_vec);
            free(jni_struct->doubleArrayArgs);
            free(jni_struct);
            write(pipe_close[1], &flag_close, sizeof(int));
            exit(EXIT_SUCCESS);
        }

        // update joint positions
        for(i=0; i<nb_models; i++)
        {
            read(pipe_q_vec[0], q_vec[i], nb_q[i]*sizeof(double));

            // copy the current values of q_vec to the jni array
            (*env)->SetDoubleArrayRegion(env, jni_struct->doubleArrayArgs[i], 0, nb_q[i], q_vec[i]);

            // update the 3D view
            (*env)->CallObjectMethod(env, jni_struct->obj, jni_struct->updateJtMultiMethod, i, jni_struct->doubleArrayArgs[i]);
        }

        read(pipe_viewpoint[0], &flag_viewpoint, sizeof(int));

        if (flag_viewpoint >= 0)
        {
            (*env)->CallObjectMethod(env, jni_struct->obj, jni_struct->setVpMethod, flag_viewpoint);
        }
    }
}
#endif

#endif
