#
# author: Nicolas Van der Noot
# Feb 27 2014
#
# This file finds the libraries
# related to Java 3D
#
# J3D_FOUND : 1 if all required files found (0 otherwise)
# J3D_PATH  : libraries -> for linkage or during the execution (Windows)
#

if (UNIX)

if (APPLE)
	

## ---- MAC OS ---- ##

# possible paths for: 'j3dcore.jar'
set(TRIAL_PATHS_DLL
  ${ROBOTRAN_SOURCE_DIR}/mbsyspad_mac_os/lib
) 

find_path(J3D_PATH j3dcore.jar ${TRIAL_PATHS_DLL})

## ---------------- ##


else (APPLE)
	

## ---- LINUX ---- ##	

# possible paths for: 'libj3dcore-ogl.so'
set(TRIAL_PATHS_DLL
#  /usr/lib/jvm/java-8-oracle/jre/bin
  /usr/lib/jvm/java-7-openjdk-amd64/jre/bin
  /usr/lib/jvm/java-6-sun/jre/bin
  /usr/lib/jvm/java-7-openjdk-amd64/jre/i386
  /usr/lib/jni
  /usr/lib/jvm/java-openjdk/jre/bin
) 

find_path(J3D_PATH libj3dcore-ogl.so ${TRIAL_PATHS_DLL})

## --------------- ##


endif (APPLE)

else (UNIX)


## ---- WINDOWS ---- ##

# possible paths for: 'j3dcore-ogl.dll'
list_paths(TRIAL_PATHS_DLL_7 "C:/Program\ Files/Java/jdk1.7.0_" "/jre/bin" 99)
list_paths(TRIAL_PATHS_DLL_8 "C:/Program\ Files/Java/jdk1.8.0_" "/jre/bin" 99)

set(TRIAL_PATHS_DLL
  ${TRIAL_PATHS_DLL_7}
  ${TRIAL_PATHS_DLL_8}
  C:/Program\ Files/Java/jdk1.8.0/jre/bin
  C:/Program\ Files/Java/Java3D/1.5.2/bin
)

find_path(J3D_PATH j3dcore-ogl.dll ${TRIAL_PATHS_DLL})

## ----------------- ##


endif (UNIX)


# flag put to 1 if all required files are found
if (J3D_PATH)
  set(J3D_FOUND 1)
else ( )
  set(J3D_FOUND 0)
endif ( )

# outputs
mark_as_advanced(
  J3D_FOUND
  J3D_PATH 
)
