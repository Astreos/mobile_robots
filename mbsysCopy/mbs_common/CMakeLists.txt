# Copyright: (C)
# Authors: Timothee Habra & Nicolas Van der Noot
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.7)

project(MBsysC)

## --------------- ##

# project configuration
set( CMAKE_C_FLAGS_RELEASE   "-O3" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

if (WIN32)
## --- WIN32 or WIN64 DETECTION --- ##
	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )   # 64bit Windows
		SET(WIN_LIB_DIRECTORY win64_include_lib)
	else( )  # 32bit Windows
		SET(WIN_LIB_DIRECTORY win32_include_lib)
	endif( )
endif()

set( ROBOTRAN_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")

# link to useful CMake functions
set(CMAKE_AUX "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_aux")
set(CMAKE_AUX_BIN ${PROJECT_BINARY_DIR}/cmake_aux)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../conf/")

add_subdirectory (${CMAKE_AUX}/listing/ ${CMAKE_AUX_BIN}/listing/)
add_subdirectory (${CMAKE_AUX}/flags/ ${CMAKE_AUX_BIN}/flags/)
add_subdirectory (${CMAKE_AUX}/libraries/ ${CMAKE_AUX_BIN}/libraries/)

mbsysc_specific_flags() #TODO move to mbsysC cmake
flags_check()
release_debug()
definitions()

# flags
if (UNIX)
    set(CMAKE_C_FLAGS "-fPIC")
    set(CMAKE_EXE_LINKER_FLAGS "-fPIC")
endif (UNIX)

# add compilation CMake modules
add_subdirectory ( mbs_numerics/  ${CMAKE_CURRENT_BINARY_DIR}/mbs_numerics/  )
add_subdirectory ( mbs_struct/    ${CMAKE_CURRENT_BINARY_DIR}/mbs_struct/    )
add_subdirectory ( mbs_utilities/ ${CMAKE_CURRENT_BINARY_DIR}/mbs_utilities/ )
add_subdirectory ( mbs_load_xml/  ${CMAKE_CURRENT_BINARY_DIR}/mbs_load_xml/  )
add_subdirectory ( mbs_realtime/  ${CMAKE_CURRENT_BINARY_DIR}/mbs_realtime/  )
add_subdirectory ( mbs_module/    ${CMAKE_CURRENT_BINARY_DIR}/mbs_module/    )
#if(FLAG_REAL_TIME)
    
#endif()

# add parent scope if needed
get_directory_property(hasParent PARENT_DIRECTORY)

if(hasParent)
    set( LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES} PARENT_SCOPE )
    set( GSL_LIBRARIES     ${GSL_LIBRARIES}     PARENT_SCOPE )
    set( SDL2_LIBRARIES    ${SDL2_LIBRARIES}    PARENT_SCOPE )
    set( SDL2TTF_LIBRARIES ${SDL2TTF_LIBRARIES} PARENT_SCOPE )
    set( JNI_LIBRARIES     ${JNI_LIBRARIES}     PARENT_SCOPE )
    set( J3D_PATH          ${J3D_PATH}          PARENT_SCOPE )
endif()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                       CMake Package Configuration
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include directories for the library linking against LIB_MBSYSC
SET(LIB_MBSYSC_INCLUDE_DIRS "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_struct"
                            "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_load_xml"
                            "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_module"
                            "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_utilities"
                            "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_utilities/auto_output"
                            "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_realtime"
                            "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_realtime/realtime"
                            "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_realtime/sdl"
                            "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_realtime/sdl/auto_plot"
)

configure_file(LibRobotranCConfig.cmake.in "${PROJECT_BINARY_DIR}/LibRobotranCConfig.cmake" @ONLY)

if(hasParent) #compiled by standalone project
    SET(LIB_MBSYSC_INCLUDE_DIRS ${LIB_MBSYSC_INCLUDE_DIRS} PARENT_SCOPE )
    SET(LIB_MBSYSC_REALTIME ${LIB_MBSYSC_REALTIME} PARENT_SCOPE)
    SET(LIB_MBSYSC_UTILITIES ${LIB_MBSYSC_UTILITIES} PARENT_SCOPE)
endif()


