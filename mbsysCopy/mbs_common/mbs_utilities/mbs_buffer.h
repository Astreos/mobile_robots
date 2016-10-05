

 /**
  *
  * Define structures and function for storing results during simulation
  * and writing them to disk.
  * 
  * @author Nicolas Docquier 
  *
  * (c) Universite catholique de Louvain
  */

#ifndef MBS_BUFFER_h
#define MBS_BUFFER_h

#include "mbsysc_utilities_export.h"

// IDs for the buffers
enum{BUFFER_Q, BUFFER_QD, BUFFER_QDD, BUFFER_QQ, BUFFER_UX, BUFFER_UXD, 
    BUFFER_LINK_Z, BUFFER_LINK_ZD, BUFFER_LINK_F, BUFFER_VISU, BUFFER_QC, BUFFER_OTHER};

/**
 * animationR .anim file info
 */ 
typedef struct AnimInfo
{
    int save_anim; ///< 1 to save the anim file, 0 otherwise (only valid if 'save2file' is set to 1)
    int save_visu; ///< 1 to save the visu file (as in 'user_realtime_visu.c'), 0 otherwise (only valid if 'save2file' is set to 1)

    double anim_period; ///< time interval between two recorded values in .anim file
    double last_print_t; ///< last time printed in the .anim file

    char *filename; ///< anim file name

} AnimInfo;

/**
 * 
 * 
 * 
 */ 
typedef struct MbsBuffer{
    
    /** the array in which the time and the value are saved.
     *  time and value are saved contiguously. This is useful for
     *  the python interface so as to save all data in a single array
     *  with index of column corresponding to joint id (starting at 1). */
    double* tx;
    
    /** the array in which the last values of 'x' are stored */
    double* last_x;

    /** the time corresponding to 'last_x' */
    double last_t;

    /** flag: 1 if it was not yet printed */
    int flag_no_print;
    
    /** the name of the text file associated to this buffer */
    char *filename;
    
    /** indicate the index to which next values must be written */
    int index;
    
    /** the size of the buffer (i.e. the number of different value of the array that can be stored)*/
    int size;
    
    /** the size of the array that is tracked by this buffer */
    int nx;

    /** ID of the buffer */
    int id;

    /** info for the .anim file */
    AnimInfo *anim;
    
} MbsBuffer;

/*! \brief buffer used to store the user variables to output
 *
 * Compared to 'MbsBuffer', this structure is designed to be extended with new user buffer columns.
 * So, its size is not know at the beginning and is growing each time a new (i.e. new label) user variable is discovered.
 * This structure is used with the 'AutoOutput' structure.
 */
typedef struct MbsGrowingBuffer{

    double* t; ///< the array in which the time value corresponding to each value of the array are stored

    double** X; ///< the tabular (2 dimensions) in which the values are stored

    char **names; ///< the names of the user variables to save

    int index; ///< indicate the index to which next values must be written

    int size; ///< the size of the buffer (i.e. the number of different value of the array that can be stored)

    int nx; ///< the current size of the array that is tracked by this buffer

    int max_nx; ///< maximal size for nx

    const char *respath; ///< path for the results
    
} MbsGrowingBuffer;

/**
 * Allocate and initialize a MbsBuffer structure.
 * The associated text file defined by filename is (re-)initialized
 * 
 * @param filename the name of the file associated to this buffer
 * @param nx the size of the array to be tracked by this buffer
 * @param size the size of the buffer
 */ 
MBSYSC_UTILITIES_EXPORT MbsBuffer* mbs_new_buffer(char* filename, char* anim_name, int nx, int size, int id, int save_anim, int save_visu, double anim_period);

/*! \brief create a new growing buffer for the user inputs to save
 * 
 * \param[in] max_nx maximum number of user inputs to save
 * \param[in] size size of the buffer
 * \return new growing buffer initialized
 */
MBSYSC_UTILITIES_EXPORT MbsGrowingBuffer* mbs_new_growing_buffer(int max_nx, int size, const char *respath);

/**
 * Free memory of the specified MbsBuffer
 * 
 */
MBSYSC_UTILITIES_EXPORT void mbs_delete_buffer(MbsBuffer* b);

/*! \brief release memory for growing buffer
 * 
 * \param[out] b growing buffer to release
 */
MBSYSC_UTILITIES_EXPORT void mbs_delete_growing_buffer(MbsGrowingBuffer *b);

/**
 * Save the given array to the specified buffer with the specified time.
 * 
 * The content of the buffer is automatically written to the associated
 * file when the buffer is full. The buffer index is then reset to 0.
 * 
 * @param b the buffer in which to store
 * @param t the corresponding time [s]
 * @param x the array to save (value are copied) 
 * 
 */ 
MBSYSC_UTILITIES_EXPORT void mbs_buffer_save(MbsBuffer* b, double t, double *x);

/**
 * Save the given values of auto_output to the specified buffer with the specified time.
 * 
 * \param[in,out] b the growing buffer in which to store
 * \param[in] t the corresponding time [s]
 *
 * The content of the buffer is automatically written to the associated
 * file when the buffer is full. The buffer index is then reset to 0.
 */ 
MBSYSC_UTILITIES_EXPORT void mbs_growing_buffer_save(MbsGrowingBuffer* b, double t);


/**
 * Write the content of the buffer to disk. Values contained in the
 * buffer for line 0 to line index-1 are appended to the file
 * associated with this buffer.
 * 
 */
MBSYSC_UTILITIES_EXPORT void mbs_buffer_write(MbsBuffer* b);

/**
 * Write the content of the q buffer as anim file to the disk.
 */
MBSYSC_UTILITIES_EXPORT void mbs_anim_write(MbsBuffer* b);

/*! \brief write the content of the growing buffer to disk
 * 
 * \param[in] b growing buffer
 */
MBSYSC_UTILITIES_EXPORT void mbs_growing_buffer_write(MbsGrowingBuffer* b);


#endif

 
