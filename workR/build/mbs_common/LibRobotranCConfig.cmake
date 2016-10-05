
# - Config file for the Robotran MBSysC package
# It defines the following variables
#  Robotran libraries to link against
#  	* LIB_MBSYSC_MODULES
#	* LIB_MBSYSC_LOAD
#	* LIB_MBSYSC_UTILITIES
#	* LIB_MBSYSC_REALTIME
#  LIB_MBSYSC_INCLUDE_DIRS - Directories containing the headers necessary to use MBSysC libraries
#  LIB_MBSYSC_DEFINITIONS  - Definitions used to compile MBSysC libraries, shall be used as well by the project linking to the MBSysC libraries (so that headers matches)

#############
# LIBRARIES
#############

# MBSYSC_MODULES
FIND_LIBRARY(LIB_MBSYSC_MODULES MBsysC_module
    PATHS /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_module
          /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_module//Debug
          /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_module//Release
)

# MBSYSC_LOAD
FIND_LIBRARY(LIB_MBSYSC_LOAD MBsysC_loadXML
    PATHS /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_load_xml
          /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_load_xml//Debug
          /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_load_xml//Release
)

# MBSYSC_UTILITIES
FIND_LIBRARY(LIB_MBSYSC_UTILITIES MBsysC_utilities
    PATHS /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_utilities
          /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_utilities//Debug
          /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_utilities//Release
)

# MBSYSC_REALTIME
FIND_LIBRARY(LIB_MBSYSC_REALTIME MBsysC_realtime
    PATHS /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_realtime
          /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_realtime//Debug
          /home/nicolas/Téléchargements/m454_project/workR/build/mbs_common/mbs_realtime//Release
)


#############
# USEFUL
#############

# Path to Robotran common files
SET(ROBOTRAN_SOURCE_DIR /home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/../)

# Call project funstion (symbolic and user) via function pointers
SET(FLAG_PRJ_FCT_PTR OFF)

# Realtime options
SET(FLAG_REAL_TIME ON)
SET(FLAG_PLOT ON)
SET(FLAG_VISU ON)

# Shared lib compilation
SET(FLAG_SHARED_LIB OFF)

#############
# INCLUDE DIRECTORIES
#############

SET(LIB_MBSYSC_INCLUDE_DIRS /home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_struct;/home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_load_xml;/home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_module;/home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_utilities;/home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_utilities/auto_output;/home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_realtime;/home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_realtime/realtime;/home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_realtime/sdl;/home/nicolas/Téléchargements/m454_project/mbsysCopy/mbs_common/..//mbs_common/mbs_realtime/sdl/auto_plot )

#############
# DEFINITIONS
#############

SET(LIB_MBSYSC_DEFINITIONS   -DUNIX;-DDIRDYNARED;-DREAL_TIME;-DSDL;-DJAVA)

