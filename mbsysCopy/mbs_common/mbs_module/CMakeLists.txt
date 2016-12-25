# Copyright: (C)
# Authors: Timothee Habra
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.7)

project(MBsysC_module)

# project configuration
set( CMAKE_C_FLAGS_RELEASE   "-O3" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

# release of debug
release_debug()

# Libxml2
xml_lib(mbs_module)
set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES} PARENT_SCOPE)

# GSL
gsl_lib(mbs_module)
set(GSL_LIBRARIES ${GSL_LIBRARIES} PARENT_SCOPE)

# list source files to compile
init_src()
increment_src( ${PROJECT_SOURCE_DIR} )

# list include directories (to find headers)
init_include()
set(INCLUDE_DIR ${INCLUDE_DIR} ${PROJECT_SOURCE_DIR} PARENT_SCOPE)

# remove some compilation warnings (This line should be removed once the loading, equil and modal modules are stabilized)
if (UNIX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-result")
endif( )

if(FLAG_SHARED_LIB)
	include (GenerateExportHeader)
    add_library(MBsysC_module SHARED ${SOURCE_FILES} ${INCLUDE_DIR})
    target_link_libraries(MBsysC_module MBsysC_struct MBsysC_numerics ${LIB_MBSYSC_UTILITIES} ${LIB_MBSYSC_REALTIME} ${LIBXML2_LIBRARIES} ${GSL_LIBRARIES})
	
	GENERATE_EXPORT_HEADER( MBsysC_module )
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	install (	TARGETS MBsysC_module DESTINATION ${CMAKE_INSTALL_PREFIX})

	add_definitions( -DPROJECT_USERFCT_STATIC_DEFINE)
		
else( )
    add_library(MBsysC_module STATIC ${SOURCE_FILES} ${INCLUDE_DIR})
	add_definitions( -DMBSYSC_UTILITIES_STATIC_DEFINE)
	add_definitions( -DPROJECT_USERFCT_STATIC_DEFINE)
	add_definitions( -DPROJECT_SYMBOLIC_STATIC_DEFINE)
	add_definitions( -DMBSYSC_REALTIME_STATIC_DEFINE)
	# add_definitions( -DMBSYSC_MODULE_STATIC_DEFINE)
endif( )

set(LIB_MBSYSC_MODULES_PATH ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

# include directories
include_directories("./")
include_directories(../mbs_struct)
include_directories(../mbs_numerics)
include_directories(../mbs_utilities)
include_directories(../mbs_utilities/auto_output)
include_directories(../mbs_realtime)
include_directories(../mbs_realtime/realtime)
include_directories(../mbs_realtime/sdl)
include_directories(../mbs_load_xml)  #this dependencies should be avoided (needed by equil which needs mds)
