# Copyright: (C)
# Authors: Timothee Habra
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.7)

project(MBsysC_loadXML)

# project configuration
set( CMAKE_C_FLAGS_RELEASE   "-O3" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

# release of debug
release_debug()

# Libxml2
xml_lib(mbs_load_xml)
set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES} PARENT_SCOPE)

# list source files to compile
init_src()
increment_src( ${PROJECT_SOURCE_DIR} )
# remove the library loading in case the fla g is not active (monolithic build)
if(NOT FLAG_PRJ_FCT_PTR)
    list(REMOVE_ITEM SOURCE_FILES ${PROJECT_SOURCE_DIR}/mbs_load_libraries.c)
endif( )

# list include directories (to find headers)
init_include()
set(INCLUDE_DIR ${INCLUDE_DIR} ${PROJECT_SOURCE_DIR} PARENT_SCOPE)

if(FLAG_SHARED_LIB)
	include (GenerateExportHeader)
    add_library(MBsysC_loadXML SHARED ${SOURCE_FILES} ${INCLUDE_DIR})
    target_link_libraries(MBsysC_loadXML ${LIBXML2_LIBRARIES} ${LIB_MBSYSC_UTILITIES})
	#target_link_libraries(MBsysC_loadXML Project_userfct )
	GENERATE_EXPORT_HEADER( MBsysC_loadXML	)
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	install (TARGETS MBsysC_loadXML DESTINATION ${CMAKE_INSTALL_PREFIX})
else( )
    add_library(MBsysC_loadXML STATIC ${SOURCE_FILES} ${INCLUDE_DIR})
	add_definitions( -DPROJECT_USERFCT_STATIC_DEFINE)
endif( )



set(LIB_MBSYSC_LOAD_PATH ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

# include directories
include_directories("./")
include_directories("../mbs_utilities")
include_directories("../mbs_struct")
