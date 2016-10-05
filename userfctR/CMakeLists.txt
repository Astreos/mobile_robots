# Copyright: (C)
# Authors: Timothee Habra & Nicolas Van der Noot

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                       LIBRARY MAIN CONFIGURATIONS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

cmake_minimum_required(VERSION 2.8.7)

project(Project_userfct)

# flags
if (UNIX)
    set(CMAKE_C_FLAGS "-fPIC")
    set(CMAKE_EXE_LINKER_FLAGS "-fPIC")
endif (UNIX)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                        SEPARATE COMPILATION
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

if ( FLAG_SEPARATE_USER_FCT )

    # additional CMakeLists.txt
    set(CMAKE_AUX ${ROBOTRAN_SOURCE_DIR}/cmake_aux)
    set(CMAKE_AUX_BIN ${PROJECT_BINARY_DIR}/cmake_aux)

    add_subdirectory ( ${CMAKE_AUX}/flags/   ${CMAKE_AUX_BIN}/flags/   )
    add_subdirectory ( ${CMAKE_AUX}/listing/ ${CMAKE_AUX_BIN}/listing/ )
    add_subdirectory ( ${CMAKE_AUX}/libraries/ ${CMAKE_AUX_BIN}/libraries/ )

    # real-time option
    option (FLAG_REAL_TIME "Real time" OFF)

    if(FLAG_REAL_TIME)
        add_definitions( -DREAL_TIME )

        # configure a header file to pass some of the CMake settings to the source code
        configure_file (
            "${ROBOTRAN_SOURCE_DIR}/conf/cmake_config.h.in"
            "${PROJECT_BINARY_DIR}/conf/cmake_config.h"
        )
        include_directories (${PROJECT_BINARY_DIR}/conf)

        # plot-visu options
        option (FLAG_PLOT "Real time" OFF)
        option (FLAG_VISU "Real time" OFF)

        if(FLAG_PLOT)
            add_definitions( -DSDL )
        endif( )

        if(FLAG_VISU)
            add_definitions( -DJAVA )
        endif( )

    endif ( )
endif ( )

# release of debug
release_debug()

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         SET LIBRARY
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# list source files to compile
init_src()
increment_src ( ${PROJECT_SOURCE_DIR} )
increment_src ( ${PROJECT_SOURCE_DIR}/../userFiles )

# list include directories (to find headers)
init_include()
increment_include( ${ROBOTRAN_SOURCE_DIR}/mbs_common )
increment_include( ${PROJECT_SOURCE_DIR}/../userFiles )

# include these directories
include_directories ( ${INCLUDE_DIR} )
include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

if( FLAG_SHARED_LIB OR FLAG_SEPARATE_USER_FCT )
    add_library(Project_userfct SHARED ${SOURCE_FILES})

    target_link_libraries (Project_userfct ${LIB_MBSYSC_REALTIME})
else()
    add_library(Project_userfct STATIC ${SOURCE_FILES})
    add_definitions( -DMBSYSC_UTILITIES_STATIC_DEFINE)
    add_definitions( -DPROJECT_SYMBOLIC_STATIC_DEFINE)

    target_link_libraries (Project_userfct ${LIB_MBSYSC_REALTIME} ${LIB_MBSYSC_UTILITIES})
endif()

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         LINK LIBRARIES
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# MBSysC libraries
if ( FLAG_SEPARATE_BUILD ) # find MBSysC dynamic libraries
    find_package( LibRobotranC REQUIRED )
    add_definitions(${LIB_MBSYSC_DEFINITIONS})
endif()

if (FLAG_PLOT)
    sdl_header_lib(userfct)
endif ( )

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         STUFF FOR WINDOWS (TO BE CLEANED)
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

include (GenerateExportHeader)
GENERATE_EXPORT_HEADER( Project_userfct )

install (TARGETS Project_userfct DESTINATION ${CMAKE_INSTALL_PREFIX})
