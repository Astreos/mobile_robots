# Copyright: (C)
# Authors: Timothee Habra
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.7)

project(MBsysC_struct)

# project configuration
set( CMAKE_C_FLAGS_RELEASE   "-O3" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

# release of debug
release_debug()

# list source files to compile
init_src()
increment_src( ${PROJECT_SOURCE_DIR} )

# list include directories (to find headers)
init_include()
set(INCLUDE_DIR ${INCLUDE_DIR} ${PROJECT_SOURCE_DIR} PARENT_SCOPE)

add_library(MBsysC_struct ${SOURCE_FILES} ${INCLUDE_DIR})

if (NOT FLAG_SEPARATE_BUILD )
	if (NOT FLAG_SHARED_LIB)
		add_definitions( -DMBSYSC_UTILITIES_STATIC_DEFINE)
	endif()
endif()

# include directories
include_directories("./")
include_directories("../mbs_utilities/")
include_directories("../mbs_numerics/")
