# Copyright: (C)
# Authors: Timothee Habra
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.7)

project(MBsysC_realtime)

# project configuration
set( CMAKE_C_FLAGS_RELEASE   "-O3" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

# release of debug
release_debug()

# list source files to compile
init_src()
increment_src( ${PROJECT_SOURCE_DIR}/realtime )
increment_src( ${PROJECT_SOURCE_DIR}/void )

# Get SDL lib and add plot functions to src
if (FLAG_PLOT)

    # Find SDL
    sdl_lib(mbs_realtime)
    set(SDL2_LIBRARIES ${SDL2_LIBRARIES} PARENT_SCOPE)
    set(SDL2TTF_LIBRARIES ${SDL2TTF_LIBRARIES} PARENT_SCOPE)

    increment_src( ${PROJECT_SOURCE_DIR}/sdl )

    # configure a header file to pass some of the CMake settings to the source code
    set(SDL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/sdl/SDL_files)

    configure_file (
        "./sdl/conf/cmake_sdl_config.h.in"
        "${PROJECT_BINARY_DIR}/conf/cmake_sdl_config.h"
    )
    include_directories ("${PROJECT_BINARY_DIR}/conf")
endif ()

# Get java lib and add java functions to src
if (FLAG_VISU)

    # find java and java 3d
    java_lib(mbs_realtime)
    set(JNI_LIBRARIES ${JNI_LIBRARIES} PARENT_SCOPE)
    set (J3D_PATH ${J3D_PATH} PARENT_SCOPE)

    include_directories("./java")
    increment_src( ${PROJECT_SOURCE_DIR}/java )

    if (APPLE)
        set(JAR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../mbsyspad_mac_os/MBsysPad.jar)
    else ( )
        set(JAR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../mbsyspad/MBsysPad.jar)
    endif ( )

    configure_file (
        "./java/conf/cmake_java_config.h.in"
        "${PROJECT_BINARY_DIR}/conf/cmake_java_config.h"
    )
    include_directories ("${PROJECT_BINARY_DIR}/conf")
endif ()

# list include directories (to find headers)
init_include()
set(INCLUDE_DIR ${INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/realtime PARENT_SCOPE)

set(LIB_MBSYSC_REALTIME MBsysC_realtime)
set(LIB_MBSYSC_REALTIME ${LIB_MBSYSC_REALTIME} PARENT_SCOPE)


if(FLAG_SHARED_LIB)
	add_library(${LIB_MBSYSC_REALTIME} SHARED ${SOURCE_FILES} ${INCLUDE_DIR})			
else()
	add_library(${LIB_MBSYSC_REALTIME} STATIC ${SOURCE_FILES} ${INCLUDE_DIR})
	add_definitions( -DPROJECT_USERFCT_STATIC_DEFINE)
	add_definitions( -DPROJECT_UTILITIES_STATIC_DEFINE)
endif()

if(FLAG_PRJ_FCT_PTR)
	add_definitions( -DPROJECT_USERFCT_STATIC_DEFINE)
	target_link_libraries (${LIB_MBSYSC_REALTIME} MBsysC_struct) 
endif()

target_link_libraries (${LIB_MBSYSC_REALTIME} ${LIB_MBSYSC_UTILITIES}) 

include (GenerateExportHeader)
GENERATE_EXPORT_HEADER( ${LIB_MBSYSC_REALTIME}	)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
install (TARGETS ${LIB_MBSYSC_REALTIME} DESTINATION ${CMAKE_INSTALL_PREFIX})

# SDL external library: linkage
if (FLAG_PLOT)
    target_link_libraries (${LIB_MBSYSC_REALTIME} ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES})
endif ( )

# Java external library: linkage
if (FLAG_VISU)
    target_link_libraries (${LIB_MBSYSC_REALTIME} ${JNI_LIBRARIES})
endif ( )

set(LIB_MBSYSC_REALTIME_PATH ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

# include directories
include_directories("./")
include_directories("./sdl" "./sdl/auto_plot")
include_directories("./realtime")
include_directories("../mbs_struct")
include_directories("../mbs_utilities")
