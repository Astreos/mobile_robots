

cmake_minimum_required(VERSION 2.8.7)

project(MBsysC_mbs_lmgc)

set(CMAKE_AUX "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake_aux")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../conf/")

set(ROBOTRAN_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../")



# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#        Compile the Robotran-LMGC90 binding
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

message(STATUS "Building the mbs_lmgc90 binding.")

# Compilation for LMGC90
set(ROBOTRAN_LMGC90_LIB_NAME "Robotran_Lmgc90")

set(libMbsFullPath "${PROJECT_BINARY_DIR}/lib${ROBOTRAN_LMGC90_LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")
#target_link_libraries( ${libMbsFullPath} MBsysC_loadXML MBsysC_module )

set(project_libraries m ${libMbsFullPath} ${LIBXML2_LIBRARIES} ${GSL_LIBRARIES} )

# generate library for lmgc90
add_library (${ROBOTRAN_LMGC90_LIB_NAME} SHARED ${SOURCE_FILES} "${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_add_on/mbs_lmgc/mbs_lmgc.c")

# get the path where MBsysC modules were compiled
find_path(LibRobotranC_DIR LibRobotranCConfig.cmake "${ROBOTRAN_SOURCE_DIR}/build")
# get the MBsysC modules libraries
find_package( LibRobotranC REQUIRED )
target_link_libraries( ${ROBOTRAN_LMGC90_LIB_NAME} ${LIB_MBSYSC_MODULES} ${LIB_MBSYSC_LOAD} ${LIB_MBSYSC_UTILITIES} )
add_definitions(${LIB_MBSYSC_DEFINITIONS})

# include header from MBsysC modules
include_directories(${LIB_MBSYSC_INCLUDE_DIRS})
include_directories(${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_numerics)

# write the configuration file for linking LMGC90 to this mbs_lmgc interface
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Robotran_Lmgc90.cfg.in
               ${CMAKE_BINARY_DIR}/Robotran_Lmgc90.cfg)

# copy the lilb from the current build directory to the main build dir (usefull only if this CMakeLists is called from another one)
add_custom_command(TARGET ${ROBOTRAN_LMGC90_LIB_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy
                             ${CMAKE_CURRENT_BINARY_DIR}/lib${ROBOTRAN_LMGC90_LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                             ${CMAKE_BINARY_DIR}/lib${ROBOTRAN_LMGC90_LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                    )

message(STATUS "Building the mbs_lmgc90 binding. --> DONE")


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#        Compile the Robotran-LMGC90 python wrapping
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

message(STATUS "Building the cython interca to the mbs_lmgc90 binding.")

# get numpy and cyhton informations
find_package(Python REQUIRED)

# add python and numpy headers
include_directories(${NUMPY_INCLUDE_DIR}
                      ${PYTHON_INCLUDE_DIR}
                      ${CMAKE_CURRENT_SOURCE_DIR}
                     )

# add a command for cythonizing the py_mbs_lmgc.pyx file to py_mbs_lmgc.c
add_custom_command(OUTPUT py_mbs_lmgc.c COMMAND ${CYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/py_mbs_lmgc.pyx -o ${CMAKE_CURRENT_BINARY_DIR}/py_mbs_lmgc.c
                     DEPENDS py_mbs_lmgc.pyx)

# add a library for compiling the py_mbs_lmgc.c
add_library(py_mbs_lmgc SHARED ${SOURCE_FILES}  ${CMAKE_CURRENT_BINARY_DIR}/py_mbs_lmgc.c)
target_link_libraries( py_mbs_lmgc ${ROBOTRAN_LMGC90_LIB_NAME}  )

# copy the generated library to main build dir and adapt the name for enabling module import from python
add_custom_command(TARGET py_mbs_lmgc POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy
                             ${CMAKE_CURRENT_BINARY_DIR}/libpy_mbs_lmgc${CMAKE_SHARED_LIBRARY_SUFFIX}
                             ${CMAKE_BINARY_DIR}/py_mbs_lmgc.so
                    )
                    
message(STATUS "Building the cython interca to the mbs_lmgc90 binding. --> DONE")
                    
