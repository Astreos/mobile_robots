# Copyright: (C)
# Authors: Timothee Habra & Nicolas Van der Noot
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                       LIBRARY MAIN CONFIGURATIONS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

cmake_minimum_required(VERSION 2.8.7)

project(Project_symbolic)

# flags
if (UNIX)
    set(CMAKE_C_FLAGS "-fPIC")
    set(CMAKE_EXE_LINKER_FLAGS "-fPIC")
endif (UNIX)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                        SEPARATE COMPILATION
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

if ( FLAG_SEPARATE_SYMBOLIC )
	set(CMAKE_AUX ${ROBOTRAN_SOURCE_DIR}/cmake_aux)
	set(CMAKE_AUX_BIN ${PROJECT_BINARY_DIR}/cmake_aux)

	add_subdirectory ( ${CMAKE_AUX}/flags/   ${CMAKE_AUX_BIN}/flags/   )
	add_subdirectory ( ${CMAKE_AUX}/listing/ ${CMAKE_AUX_BIN}/listing/ )
endif ( )

# release of debug
release_debug()

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         SET LIBRARY
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# list source files to compile
init_src()
increment_src( ${PROJECT_SOURCE_DIR} )
increment_void_symbolic( ${PROJECT_SOURCE_DIR} )

# list include directories (to find headers)
init_include()
increment_include( ${ROBOTRAN_SOURCE_DIR}/mbs_common )

# include these directories
include_directories ( ${INCLUDE_DIR} )
include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

if( FLAG_SHARED_LIB OR FLAG_SEPARATE_SYMBOLIC )
    add_library(Project_symbolic SHARED ${SOURCE_FILES})
else()
    add_library(Project_symbolic STATIC ${SOURCE_FILES})
    add_definitions( -DPROJECT_USERFCT_STATIC_DEFINE)

    # some symbolic routines call user functions (ext_forces, accelred, ...)
    target_link_libraries(Project_symbolic Project_userfct)
endif()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         LINK LIBRARIES
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# MBSysC libraries
if ( FLAG_SEPARATE_BUILD ) # find MBSysC dynamic libraries
    find_package( LibRobotranC REQUIRED )
    add_definitions(${LIB_MBSYSC_DEFINITIONS})
endif()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         STUFF FOR WINDOWS (TO BE CLEANED)
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

include (GenerateExportHeader)
GENERATE_EXPORT_HEADER( Project_symbolic    )

install (TARGETS Project_symbolic DESTINATION ${CMAKE_INSTALL_PREFIX})
