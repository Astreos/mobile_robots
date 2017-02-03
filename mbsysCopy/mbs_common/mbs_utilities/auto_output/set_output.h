/*! 
 * \author Nicolas Van der Noot
 * \file set_output.h
 * \brief set_output function called by the user to output his own variables
 */

#ifndef _SET_OUTPUT_H_
#define _SET_OUTPUT_H_

#include "mbsysc_utilities_export.h"

// funcions prototype
#ifdef __cplusplus // in case the function is called from a C++ file
    extern "C" {
        MBSYSC_UTILITIES_EXPORT void set_output(double value, const char* label);
    }
#else
    MBSYSC_UTILITIES_EXPORT void set_output(double value, char* label);
#endif

MBSYSC_UTILITIES_EXPORT void init_set_output(int max_nb_curves);
MBSYSC_UTILITIES_EXPORT void free_set_output();

double get_output_value(int index);
char* get_output_label(int index);
int get_output_nb();
int get_output_nb_new();
void set_output_nb_new(int nb);

#endif
