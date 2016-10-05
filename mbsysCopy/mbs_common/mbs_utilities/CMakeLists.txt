# Copyright: (C)
# Authors: Timothee Habra
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.7)

project(MBsysC_utilities)

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
set(INCLUDE_DIR ${INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/auto_output PARENT_SCOPE)

set(LIB_MBSYSC_UTILITIES MBsysC_utilities)
set(LIB_MBSYSC_UTILITIES ${LIB_MBSYSC_UTILITIES} PARENT_SCOPE)

if(FLAG_SHARED_LIB)
	include (GenerateExportHeader)
    add_library(${LIB_MBSYSC_UTILITIES} SHARED ${SOURCE_FILES} ${INCLUDE_DIR})
    target_link_libraries(MBsysC_struct)
	
	GENERATE_EXPORT_HEADER( ${LIB_MBSYSC_UTILITIES}	)
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	install (TARGETS ${LIB_MBSYSC_UTILITIES} DESTINATION ${CMAKE_INSTALL_PREFIX})
else( )
    add_library(${LIB_MBSYSC_UTILITIES} STATIC ${SOURCE_FILES} ${INCLUDE_DIR})
	add_definitions( -DMBSYSC_UTILITIES_STATIC_DEFINE)	
endif( )

set(LIB_MBSYSC_UTILITIES_PATH ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

# include directories
include_directories("./")
include_directories("./auto_output")
include_directories(../mbs_struct)
