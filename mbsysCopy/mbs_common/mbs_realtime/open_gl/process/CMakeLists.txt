#
# author: Nicolas Van der Noot
#
# Visualization project for Robotran, with OpenGL, to be used in a separate process
#

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                       PROJECT MAIN CONFIGURATIONS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


# CMake minimum version
cmake_minimum_required(VERSION 2.8.12)

# project name
project (OpenGLRobotranProcess)


# message to display the project name
message(STATUS "Processing ${PROJECT_NAME}")

# for Unix: display all the warnings, except the ones related to /* -- */
if (UNIX)
  set(CMAKE_C_FLAGS "-g -Wall -Wno-comment")
endif (UNIX)

# copy dll files used for Windows
# these dll files are copied in the Executable directory (Debug or Release)
if (WIN32)

    # avoid warnings
    if(COMMAND cmake_policy)
        cmake_policy(SET CMP0054 NEW)
    endif()

    # Visual studio version
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 18.0)
            message("Visual Studio version set to VS2012 (${CMAKE_CXX_COMPILER_VERSION})")
            set(MSVC_VERSION lib-vc2012)
        elseif (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 19.0)
            message("Visual Studio version set to VS2013 (${CMAKE_CXX_COMPILER_VERSION})")
            set(MSVC_VERSION lib-vc2013)
        else ( )
            message("Visual Studio version set to VS2015 (${CMAKE_CXX_COMPILER_VERSION})")
            set(MSVC_VERSION lib-vc2015)
        endif ( )
    else ( )
        message(FATAL_ERROR "CMake error: only Visual Studio projects can currently be used on Windows !")
    endif ( )

    # WIN32 or WIN64 detetction
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 ) # 64bit Windows
        SET(WIN_LIB_DIRECTORY win64_include_lib)
    else( ) # 32bit Windows
        SET(WIN_LIB_DIRECTORY win32_include_lib)
    endif( )

    # common dll
    file(COPY ${PROJECT_SOURCE_DIR}/../../../../${WIN_LIB_DIRECTORY}/dll/ DESTINATION ${CMAKE_BINARY_DIR}/Debug)
    file(COPY ${PROJECT_SOURCE_DIR}/../../../../${WIN_LIB_DIRECTORY}/dll/ DESTINATION ${CMAKE_BINARY_DIR}/Release)

    # GLFW
    file(COPY ${PROJECT_SOURCE_DIR}/../../../../${WIN_LIB_DIRECTORY}/dll/GLFW/${MSVC_VERSION}/ DESTINATION ${CMAKE_BINARY_DIR}/Debug)
    file(COPY ${PROJECT_SOURCE_DIR}/../../../../${WIN_LIB_DIRECTORY}/dll/GLFW/${MSVC_VERSION}/ DESTINATION ${CMAKE_BINARY_DIR}/Release)
endif ( )

# additional CMake functions
set(CMAKE_AUX ${PROJECT_SOURCE_DIR}/../../../../cmake_aux)
set(CMAKE_AUX_BIN ${PROJECT_BINARY_DIR}/cmake_aux)

add_subdirectory ( ${CMAKE_AUX}/listing/   ${CMAKE_AUX_BIN}/listing/   )
add_subdirectory ( ${CMAKE_AUX}/libraries/ ${CMAKE_AUX_BIN}/libraries/ )

# UNIX system
if (UNIX)
    add_definitions(-D UNIX)
endif (UNIX)

# Windows M_PI definitions
if (WIN32)
    add_definitions(-D _USE_MATH_DEFINES)
endif (WIN32)

# project own definition
add_definitions(-D PROCESS_OPEN_GL)



# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                           USER ABSOLUTE PATHS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# configure header files to pass some of the CMake settings to the source code
set (ROBOTRAN_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../../..")

configure_file (
  "${PROJECT_SOURCE_DIR}/../../../../conf/cmake_config.h.in"
  "${PROJECT_BINARY_DIR}/conf/cmake_config.h"
)

set(OPEN_GL_FILES_PATH ${PROJECT_SOURCE_DIR}/../mbs_common/mbs_realtime/open_gl/)

configure_file (
    "${PROJECT_SOURCE_DIR}/../conf/cmake_open_gl_config.h.in"
    "${PROJECT_BINARY_DIR}/conf/cmake_open_gl_config.h"
)

# 'cmake_config.h.in' and 'cmake_open_gl_config.h.in' is in the 'conf' folder
include_directories (${PROJECT_BINARY_DIR}/conf)



# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                           EXTERNAL LIBRARIES
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../../../conf)

# Libxml2
xml_lib(main)

# OpenGL
open_gl_lib(main)


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                           LIST FILES
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 
# list source files to compile
init_src()

increment_src("${PROJECT_SOURCE_DIR}/..")


# list include directories (to find headers)
init_include()

increment_include("${PROJECT_SOURCE_DIR}/..")



# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         CONFIGURE EXECUTABLE
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

# include these directories
include_directories (${INCLUDE_DIR})

# name of the executable
set (Executable open_gl_process)

# generate the executable
add_executable (${Executable} ${SOURCE_FILES})

# link library: Libxml2
target_link_libraries ( ${Executable} ${LIBXML2_LIBRARIES} )

# link library: OpenGL
target_link_libraries ( ${Executable} ${OPEN_GL_LIBS})

# math external library (for Unix): linkage
if (UNIX)
    target_link_libraries (${Executable} m)
endif (UNIX)
