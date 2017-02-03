#
# author: Nicolas Van der Noot
#
# This file finds the libraries used for separate builds
# when they are located at their standard places.
#
# This file is currently only configured for Linux !
#
# SEPARATE_MBS_COMMON_FOUND : 1 if all libraries found for mbs_common
# SEPARATE_SYMBOLIC_FOUND   : 1 if symbolic library found
# SEPARATE_USER_FCT_FOUND   : 1 if user fonctions library found
# ALL_SEPARATE_LIBS_FOUND   : 1 if all separate libraries found
#

## -- MBS_COMMON -- ##

# possible paths for: 'libMBsysC_numerics.a'
set(TRIAL_PATHS_LIB_MBSYSC_NUMERICS
    "${ROBOTRAN_SOURCE_DIR}/build/mbs_numerics"
)

# possible paths for: 'libMBsysC_struct.a'
set(TRIAL_PATHS_LIB_MBSYSC_STRUCT
    "${ROBOTRAN_SOURCE_DIR}/build/mbs_struct"
)

# possible paths for: 'libMBsysC_utilities.so'
set(TRIAL_PATHS_LIB_MBSYSC_UTILITIES
    "${ROBOTRAN_SOURCE_DIR}/build/mbs_utilities"
)
# possible paths for: 'libMBsysC_loadXML.so'
set(TRIAL_PATHS_LIB_MBSYSC_LOAD_XML
    "${ROBOTRAN_SOURCE_DIR}/build/mbs_load_xml"
)

# possible paths for: 'libMBsysC_realtime.so'
set(TRIAL_PATHS_LIB_MBSYSC_REALTIME
    "${ROBOTRAN_SOURCE_DIR}/build/mbs_realtime"
)

# possible paths for: 'libMBsysC_module.so'
set(TRIAL_PATHS_LIB_MBSYSC_MODULE
    "${ROBOTRAN_SOURCE_DIR}/build/mbs_module"
)


## -- SYMBOLIC -- ##

# possible paths for: 'libProject_symbolic.so'
set(TRIAL_PATHS_LIB_SYMBOLIC
    "${PROJECT_SOURCE_DIR}/../${SYMBOLIC_PATH}/build"
    "${PROJECT_SOURCE_DIR}/../symbolicR/build"
)


## -- USER_FCT -- ##

# possible paths for: 'libProject_userfct.so'
set(TRIAL_PATHS_LIB_USER_FCT
    "${PROJECT_SOURCE_DIR}/../userfctR/build"
)


# -- FIND THE LIBRARIES -- ##

# unset old configurations
unset( LIB_MBSYSC_NUMERICS  CACHE )
unset( LIB_MBSYSC_STRUCT    CACHE )
unset( LIB_MBSYSC_UTILITIES CACHE )
unset( LIB_MBSYSC_LOAD_XML  CACHE )
unset( LIB_MBSYSC_REALTIME  CACHE )
unset( LIB_MBSYSC_MODULE    CACHE )
unset( LIB_SYMBOLIC CACHE )
unset( LIB_USER_FCT CACHE )

# mbs_common
find_library( LIB_MBSYSC_NUMERICS  libMBsysC_numerics.a   ${TRIAL_PATHS_LIB_MBSYSC_NUMERICS}  )
find_library( LIB_MBSYSC_STRUCT    libMBsysC_struct.a     ${TRIAL_PATHS_LIB_MBSYSC_STRUCT}    )
find_library( LIB_MBSYSC_UTILITIES libMBsysC_utilities.so ${TRIAL_PATHS_LIB_MBSYSC_UTILITIES} )
find_library( LIB_MBSYSC_LOAD_XML  libMBsysC_loadXML.so   ${TRIAL_PATHS_LIB_MBSYSC_LOAD_XML}  )
find_library( LIB_MBSYSC_REALTIME  libMBsysC_realtime.so  ${TRIAL_PATHS_LIB_MBSYSC_REALTIME}  )
find_library( LIB_MBSYSC_MODULE    libMBsysC_module.so    ${TRIAL_PATHS_LIB_MBSYSC_MODULE}    )

# symbolic
find_library( LIB_SYMBOLIC libProject_symbolic.so ${TRIAL_PATHS_LIB_SYMBOLIC} )

# user fonctions
find_library( LIB_USER_FCT libProject_userfct.so ${TRIAL_PATHS_LIB_USER_FCT} )


# -- CHECK IF LIBRARIES ARE FOUND -- #

# flag put to 1 if all required libraries are found for mbs_common libraries
if (LIB_MBSYSC_NUMERICS AND LIB_MBSYSC_STRUCT AND LIB_MBSYSC_UTILITIES AND LIB_MBSYSC_LOAD_XML AND LIB_MBSYSC_REALTIME AND LIB_MBSYSC_MODULE)
    set(SEPARATE_MBS_COMMON_FOUND 1)
else ( )
    set(SEPARATE_MBS_COMMON_FOUND 0)
endif ( )

# flag put to 1 if symbolic library found
if (LIB_SYMBOLIC)
    set(SEPARATE_SYMBOLIC_FOUND 1)
else ( )
    set(SEPARATE_SYMBOLIC_FOUND 0)
endif ( )

# flag put to 1 if user fonctions library found
if (LIB_USER_FCT)
    set(SEPARATE_USER_FCT_FOUND 1)
else ( )
    set(SEPARATE_USER_FCT_FOUND 0)
endif ( )

# flag put to 1 if all libraries are found
if (SEPARATE_MBS_COMMON_FOUND AND SEPARATE_SYMBOLIC_FOUND AND SEPARATE_USER_FCT_FOUND)
    set(ALL_SEPARATE_LIBS_FOUND 1)
else ( )
    set(ALL_SEPARATE_LIBS_FOUND 0)
endif ( )


# -- OUTPUTS -- #

mark_as_advanced(
    SEPARATE_MBS_COMMON_FOUND
    SEPARATE_SYMBOLIC_FOUND
    SEPARATE_USER_FCT_FOUND
    ALL_SEPARATE_LIBS_FOUND
)
