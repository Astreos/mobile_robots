#
# Author: Nicolas Van der Noot & Timothee Habra
# September 2015
#
# Listing functions 
#

## -- SRC FILES -- ##

# function to list all files whose form equals arg1,
# recursing into the folder arg2 (excluding source files inside a 'build folder' 
# to prevent CMake incompatibilities)
# -> arg0 contains the list at the end
function(list_files arg0 arg1 arg2)
    file(GLOB_RECURSE SOURCE_FILES_TMP  
            "${arg2}/${arg1}")
    set(CUR_SRC_LIST "")
    foreach(FILE_PATH ${SOURCE_FILES_TMP})
        string(REPLACE ${arg2} "" END_FILE ${FILE_PATH})
        string(FIND ${END_FILE} "build" BUILD_PLACE)
        if (${BUILD_PLACE} LESS 0)
            set(CUR_SRC_LIST ${CUR_SRC_LIST} ${FILE_PATH})
        endif ()
    endforeach(FILE_PATH)
    set(${arg0} ${CUR_SRC_LIST} PARENT_SCOPE)
endfunction()

# list all source files (.c, .cc, .cpp) 
# recursively contained into the folder arg1
# -> arg0 contains the list at the end
function(list_src_files arg0 arg1)
    set(CUR_FULL_LIST "")
    list_files(CUR_LIST "*.c" ${arg1})
    set(CUR_FULL_LIST ${CUR_LIST})
    list_files(CUR_LIST "*.cc" ${arg1})
    set(CUR_FULL_LIST ${CUR_FULL_LIST} ${CUR_LIST})
    list_files(CUR_LIST "*.cpp" ${arg1})
    set(CUR_FULL_LIST ${CUR_FULL_LIST} ${CUR_LIST})
    set(${arg0} ${CUR_FULL_LIST} PARENT_SCOPE)
endfunction()

# reset SOURCE_FILES
function(init_src)
    set(SOURCE_FILES "" PARENT_SCOPE)
endfunction()

# increment SOURCE_FILES with the files in the arg0 folder (recursively)
function(increment_src arg0)
    list_src_files(CUR_SOURCE_FILES ${arg0})
    set(SOURCE_FILES ${SOURCE_FILES} ${CUR_SOURCE_FILES} PARENT_SCOPE)
endfunction()

# increment SOURCE_FILES with the arg0 file (specific source file)
function(increment_src_file arg0)
    set(SOURCE_FILES ${SOURCE_FILES} ${arg0} PARENT_SCOPE)
endfunction()


## -- SRC FILES DIRECTORIES -- ##

# function to list all the directories containing source files
# with arg1 extensions, recursing into the folder arg2
# -> arg0 contains the list at the end
function(list_sub_dir arg0 arg1 arg2)
    file(GLOB_RECURSE SOURCE_TMP "${arg2}/${arg1}")
    set(CUR_DIR_LIST "")
    foreach(FILE_PATH ${SOURCE_TMP})
        get_filename_component(CUR_PATH ${FILE_PATH} PATH)
        set(FOUND_SAME 0)
        foreach(CUR_DIR ${CUR_DIR_LIST})
            if("${CUR_DIR}" STREQUAL "${CUR_PATH}")
                set(FOUND_SAME 1)
            endif ()
        endforeach(CUR_DIR)
        if (NOT FOUND_SAME)
            set(CUR_DIR_LIST ${CUR_DIR_LIST} ${CUR_PATH})
        endif ()
    endforeach(FILE_PATH)
    set(${arg0} ${CUR_DIR_LIST} PARENT_SCOPE)
endfunction()

# function to list all the directories containing C or C++ source files,
# recursing into the folder arg1
# -> arg0 contains the list at the end
function(list_src_sub_dir arg0 arg1)
    set(CUR_FULL_LIST "")
    list_sub_dir(CUR_LIST "*.c" ${arg1})
    set(CUR_FULL_LIST ${CUR_LIST})
    list_sub_dir(CUR_LIST "*.cc" ${arg1})
    set(CUR_FULL_LIST ${CUR_FULL_LIST} ${CUR_LIST})
    list_sub_dir(CUR_LIST "*.cpp" ${arg1})
    set(CUR_FULL_LIST ${CUR_FULL_LIST} ${CUR_LIST})
    set(${arg0} ${CUR_FULL_LIST} PARENT_SCOPE)
endfunction()


## -- INCLUDE DIRECTORIES -- ##

# function to list all directories containing ".h", ".hh" and ".hpp" files,
# recursing into the folder arg1
# -> arg0 contains the list at the end
function(list_include_directories arg0 arg1)
    file(GLOB_RECURSE SOURCE_FILES_TMP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
            "${arg1}/*.h"
            "${arg1}/*.hh"
            "${arg1}/*.hpp")
    set(CUR_DIR_LIST "")
    foreach(FILE_PATH ${SOURCE_FILES_TMP})
        get_filename_component(CUR_PATH ${FILE_PATH} PATH)
        set(CUR_DIR_LIST ${CUR_DIR_LIST} ${CUR_PATH})
    endforeach(FILE_PATH)
    list(REMOVE_DUPLICATES CUR_DIR_LIST) 
    set(${arg0} ${CUR_DIR_LIST} PARENT_SCOPE)
endfunction()

# reset INCLUDE_DIR
function(init_include)
    set(INCLUDE_DIR "" PARENT_SCOPE)
endfunction()

# increment INCLUDE_DIR with the files in the arg0 folder (recursively)
function(increment_include arg0)
    list_include_directories(CUR_INCLUDE_DIR ${arg0})
    set(INCLUDE_DIR ${INCLUDE_DIR} ${CUR_INCLUDE_DIR} PARENT_SCOPE)
endfunction()

# increment INCLUDE_DIR with the arg0 file (specific header file)
function(increment_include_file arg0)
    set(INCLUDE_DIR ${INCLUDE_DIR} ${arg0} PARENT_SCOPE)
endfunction()


## -- PATHS CMAKE LIBRARIES -- ##

# list path strings as "${arg1}${i}${arg2}"
# where i is in the range [0 ; arg3]
# -> arg0 contains the resulting list
function(list_paths arg0 arg1 arg2 arg3)
    set (CUR_LIST "")
    foreach(i RANGE ${arg3})
        set (CUR_LIST ${CUR_LIST} "${arg1}${i}${arg2}")
    endforeach ( )
    set (${arg0} ${CUR_LIST} PARENT_SCOPE)
endfunction ( )


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         CHECKING SYMBOLIC FILES
#
# check whether symbolic files exist in the project folder.
# Otherwise, add an empty symbolic file from common sources
# to ensure that the project will compile.
#
# 'arg0' is the full path to the folder in which to search for the symbolic files

function(increment_void_symbolic arg0)

    set(VOID_SYMBOLIC "")

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_cons_hJ*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_cons_hJ*.h")
    
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: your project does not have any constraint, linking to dummy mbs_cons_hJ symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_cons_hJ_void.c) 
    endif()

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_cons_jdqd*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_cons_jdqd*.h")
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: your project does not have any Jdqd terms, linking to dummy mbs_cons_jdqd symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_cons_jdqd_void.c)
    endif()

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_dirdyna*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_dirdyna*.h")
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: your project does not generate any direct dynamic files, linking to dummy mbs_dirdyna symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_dirdyna_void.c)
    endif()

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_accelred*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_accelred*.h")
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: you did not ask for the full symbolic generation, linking to dummy mbs_accelred symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_accelred_void.c)
    endif()

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_extforces*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_extforces*.h")
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: your project does not have any external forces defined, linking to dummy mbs_extforces symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_extforces_void.c)
    endif()

#    file(GLOB SYMBOLIC_FILES_C "${PROJECT_SOURCE_DIR}/../${arg0}/mbs_gensensor*.c")
#    file(GLOB SYMBOLIC_FILES_H "${PROJECT_SOURCE_DIR}/../${arg0}/mbs_gensensor*.h")
#    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
#        message("Symbolic mbs_gensensor not found, linking to void function")
#        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_gensensor_void.c)
#    endif()

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_invdyna*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_invdyna*.h")
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: your project does not generate any inverse dynamic files, linking to dummy mbs_invdyna symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_invdyna_void.c)
    endif()

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_link_*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_link_*.h")
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: your project does not have any link defined, linking to dummy mbs_link symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_link_void.c)
    endif()

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_link3D*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_link3D*.h")
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: your project does not have any Link3D defined, linking to dummy mbs_link3D symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_link3D_void.c)
    endif()

    file(GLOB SYMBOLIC_FILES_C "${arg0}/mbs_sensor*.c")
    file(GLOB SYMBOLIC_FILES_H "${arg0}/mbs_sensor*.h")
    if (NOT SYMBOLIC_FILES_C OR NOT SYMBOLIC_FILES_H)
        message("Information: your project does not have any sensor defined, linking to dummy mbs_sensor symbolic function.")
        set(VOID_SYMBOLIC ${VOID_SYMBOLIC} ${ROBOTRAN_SOURCE_DIR}/mbs_common/mbs_void_symbolicR/mbs_sensor_void.c)
    endif()
    
    set(SOURCE_FILES ${SOURCE_FILES} ${VOID_SYMBOLIC} PARENT_SCOPE)

endfunction()
