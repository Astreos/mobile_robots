/*! 
 * \author Robotran team
 * \file lut.h
 * \brief Interpolation of value from array (Look Up Table) main header
 */


#ifndef lut_h
#define lut_h
/*--------------------*/


// -- Structures -- //
/*! \brief Structure for interpolate a variable from one input
 */
typedef struct MbsLut1D
{
    int nx;    ///< number of element in the array
    double *x; ///< 1D array containing the input values
    double *y; ///< 1D array containing the output to be interpolate at each defined input values
} MbsLut1D;

/*! \brief Structure for interpolate a variable from two inputs
 */
typedef struct MbsLut2D
{
    int nx;      ///< number of element in the array of the first input
    int ny;      ///< number of element in the array of the second input
    double *x;   ///< 1D array containing the first input values
    double *y;   ///< 1D array containing the second input values
    double **z;  ///< 2D array containing the output to be interpolate at each defined input values
} MbsLut2D;


/*! \brief Allocate a 1D LUT (Look Up Table) structure
 * 
 * \return An allocated 1D LUT structure
 */
MbsLut1D* mbs_lut_1D_alloc();
/*! \brief Allocate a 2D LUT (Look Up Table) structure
 * 
 * \return An allocated 2D LUT structure
 */
MbsLut2D* mbs_lut_2D_alloc();


/*! \brief Release memory used by 1D LUT structure
 * 
 * \param[in,out] lut 1D LUT structure to release memory
 */
void mbs_lut_1D_free(MbsLut1D *lut);
/*! \brief Release memory used by 2D LUT structure
 * 
 * \param[in,out] lut 2D LUT structure to release memory
 */
void mbs_lut_2D_free(MbsLut2D *lut);


/*! \brief Interpolate the output from a 1D Look Up Table
 * 
 * \param[in] lut 1D LUT structure
 * \param[in] x Input value at which the output has to be interpolate
 * \return The value of the output at the specified input value
 */
double mbs_lut_1D_interp(MbsLut1D *lut, double x);
/*! \brief Interpolate the output from a 2D Look Up Table for a specific couple of input value
 * 
 * \param[in] lut 2D LUT structure
 * \param[in] x,y The couple of input values at which the output has to be interpolate
 * \return The value of the output at the specified input value
 */
double mbs_lut_2D_interp(MbsLut2D *lut, double x, double y);


#ifdef MATLAB_MEX_FILE
/*! \brief Load an mxArray and return a new allocated 1D LUT structure
 * 
 * \param[in] mxArray mxArray structure with an "x" field containing the input values and "y" field with the output values. Both are rows vectors, size = 1 x n
 * \return A new allocated 1D LUT structure with the data from mxArray
 */
MbsLut1D* mbs_lut_1D_load_mxarray(const mxArray *LUT_1D_ptr);
/*! \brief Load an mxArray and return a new allocated 2D LUT structure
 * 
 * \param[in] mxArray mxArray structure with "x" and "y" fields containing the input values and "z" field with the output values. Inputs fields are rows vectors (size "x" = 1 x m and size "y" = 1 x n). Output field "z" is a 2D array of size m x n.
 * \return A new allocated 2D LUT structure with the data from mxArray
 */
MbsLut2D* mbs_lut_2D_load_mxarray(const mxArray *LUT_2D_ptr);
#endif


/*! \brief Load a vector and return a new allocated 1D LUT structure
 * 
 * \param[in] LUT_1D_ptr 1D double array containing all the input values and then all the output values, the size is 2*nx
 * \param[in] nx         Number of input data value
 * \return A new allocated 1D LUT structure with the data of the array
 */
MbsLut1D* mbs_lut_1D_load_1vector(const double *LUT_1D_ptr, const int nx);
/*! \brief Load two vectors and return a new allocated 1D LUT structure
 * 
 * \param[in] x_ptr 1D double array containing all the input values, the size is nx
 * \param[in] y_ptr 1D double array containing all the output values, the size is nx
 * \param[in] nx       Number of input data value
 * \return A new allocated 1D LUT structure with the data of the arrays
 */
MbsLut1D* mbs_lut_1D_load_2vectors(const double *x_ptr,const double *y_ptr,const int nx);


/*! \brief Load a file and return a new allocated 1D LUT structure
 * 
 * \param[in] fileName Name of the file containing the Look Up Table to load.\n
                       The first line describe the contains of the file (first line is not read by the function).\n
                       The second line contains the number of elements in the input x (nx).\n
                       The third line contains the x input value\n
                       The fourth line contains the y ouput value for each x input                      
 * \return A new allocated 1D LUT structure with the data of the file
 */
MbsLut1D* mbs_lut_1D_load_file(char* fileName);
/*! \brief Load a file and return a new allocated 2D LUT structure
 * 
 * \param[in] fileName Name of the file containing the Look Up Table to load.\n
                       The first line describe the contains of the file (first line is not read by the function).\n
                       The second line contains the number of elements in the input x (nx) and then y (ny).\n
                       The third line contains the y input value\n
                       The fourth to (nx+4)th lines contains as first value the x input value and then the output values for each y input value (the size of each line is 1+ny)                       
 * \return A new allocated 2D LUT structure with the data of the file
 */
MbsLut2D* mbs_lut_2D_load_file(char* fileName);

#endif
