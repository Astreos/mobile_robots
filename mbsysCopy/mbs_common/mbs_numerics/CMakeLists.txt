# Copyright: (C)
# Authors: Timothee Habra
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.7)

project(MBsysC_numerics)

# project configuration
set( CMAKE_C_FLAGS_RELEASE   "-O3" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

# release of debug
release_debug()

init_src()

init_include()


# GSL
gsl_lib(mbs_numerics)
set(GSL_LIBRARIES ${GSL_LIBRARIES} PARENT_SCOPE)

# list source files to compile
increment_src( ${PROJECT_SOURCE_DIR} )

# list include directories (to find headers)
# set(INCLUDE_DIR ${INCLUDE_DIR} ${PROJECT_SOURCE_DIR} PARENT_SCOPE)

add_library(MBsysC_numerics ${SOURCE_FILES} )
target_link_libraries(MBsysC_numerics ${GSL_LIBRARIES})
# include directories
include_directories(.)
include_directories(../mbs_struct)
include_directories(../mbs_utilities)
include_directories(../mbs_module)  # necessary for thetaSC.c 
