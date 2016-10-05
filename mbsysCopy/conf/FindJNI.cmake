#
# author: Nicolas Van der Noot
# Feb 27 2014
#
# This file finds the include folders (header files) and the libraries
# related to JNI (Java Native Interface)
#
# JNI_FOUND :        1 if all required files found (0 otherwise)
# JNI_INCLUDE_PATH : include paths (for the header files) -> for compilation
# JNI_LIBRARIES :    libraries -> for linkage
#


if (UNIX)

if (APPLE)
	

## ---- MAC OS ---- ##

# possible paths for: 'jni.h'
list_paths(TRIAL_PATHS_INC_JNI_1 "/Library/Java/JavaVirtualMachines/jdk1.7.0_" ".jdk/Contents/Home/include" 200)
list_paths(TRIAL_PATHS_INC_JNI_2 "/Library/Java/JavaVirtualMachines/jdk1.8.0_" ".jdk/Contents/Home/include" 200)

set(TRIAL_PATHS_INC_JNI
  ${TRIAL_PATHS_INC_JNI_1}
  ${TRIAL_PATHS_INC_JNI_2}
  /System/Library/Frameworks/JavaVM.framework/Headers
) 

# possible paths for: 'jni_md.h'
list_paths(TRIAL_PATHS_INC_JNI_MD_1 "/Library/Java/JavaVirtualMachines/jdk1.7.0_" ".jdk/Contents/Home/include/darwin" 200)
list_paths(TRIAL_PATHS_INC_JNI_MD_2 "/Library/Java/JavaVirtualMachines/jdk1.8.0_" ".jdk/Contents/Home/include/darwin" 200)

set(TRIAL_PATHS_INC_JNI_MD
  ${TRIAL_PATHS_INC_JNI_MD_1}
  ${TRIAL_PATHS_INC_JNI_MD_2}
  /System/Library/Frameworks/JavaVM.framework/Headers
) 

# possible paths for: 'libjvm.dylib'
list_paths(TRIAL_PATHS_LIB_1 "/Library/Java/JavaVirtualMachines/jdk1.7.0_" ".jdk/Contents/Home/jre/lib/server" 200)
list_paths(TRIAL_PATHS_LIB_2 "/Library/Java/JavaVirtualMachines/jdk1.8.0_" ".jdk/Contents/Home/jre/lib/server" 200)

set(TRIAL_PATHS_LIB
  ${TRIAL_PATHS_LIB_1}
  ${TRIAL_PATHS_LIB_2}
) 

find_path(JNI_INCLUDE_JNI jni.h ${TRIAL_PATHS_INC_JNI})
find_path(JNI_INCLUDE_JNI_MD jni_md.h ${TRIAL_PATHS_INC_JNI_MD})

find_library(JNI_LIBRARIES libjvm.dylib ${TRIAL_PATHS_LIB})

set(JNI_LIBRARIES ${JNI_LIBRARIES} "-framework CoreFoundation")

set(JNI_INCLUDE_PATH ${JNI_INCLUDE_JNI} ${JNI_INCLUDE_JNI_MD})

## ---------------- ##


else (APPLE)
	

## ---- LINUX ---- ##	

# possible paths for: 'jni.h'
set(TRIAL_PATHS_INC_JNI
  /usr/lib/jvm/java-8-openjdk-amd64/include
  /usr/lib/jvm/java-7-openjdk-amd64/include
  /usr/lib/jvm/java-6-sun/include
  /usr/lib/jvm/java-openjdk/include
  /usr/lib/jvm/java-8-oracle/include
) 

# possible paths for: 'jni_md.h'
set(TRIAL_PATHS_INC_JNI_MD
  /usr/lib/jvm/java-8-openjdk-amd64/include/linux
  /usr/lib/jvm/java-7-openjdk-amd64/include/linux
  /usr/lib/jvm/java-6-sun/include/linux
  /usr/lib/jvm/java-openjdk/include/linux
  /usr/lib/jvm/java-8-oracle/include/linux
) 

# possible paths for: 'libjvm.so'
set(TRIAL_PATHS_LIB
  /usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server
  /usr/lib/jvm/java-7-openjdk-amd64/jre/lib/amd64/server
  /usr/lib/jvm/java-6-sun/jre/lib/amd64/server
  /usr/lib/jvm/java-openjdk/jre/lib/amd64/server
  /usr/lib/jvm/java-8-oracle/jre/lib/amd64/server
) 

find_path(JNI_INCLUDE_JNI jni.h ${TRIAL_PATHS_INC_JNI})
find_path(JNI_INCLUDE_JNI_MD jni_md.h ${TRIAL_PATHS_INC_JNI_MD})

find_library(JNI_LIBRARIES libjvm.so ${TRIAL_PATHS_LIB})

set(JNI_INCLUDE_PATH ${JNI_INCLUDE_JNI} ${JNI_INCLUDE_JNI_MD})

## --------------- ##


endif (APPLE)

else (UNIX)


## ---- WINDOWS ---- ##

# possible paths for: 'jni.h'
list_paths(TRIAL_PATHS_INC_JNI_7 "C:/Program\ Files/Java/jdk1.7.0_" "/include" 200)
list_paths(TRIAL_PATHS_INC_JNI_8 "C:/Program\ Files/Java/jdk1.8.0_" "/include" 200)

set(TRIAL_PATHS_INC_JNI
  ${TRIAL_PATHS_INC_JNI_7}
  ${TRIAL_PATHS_INC_JNI_8}
  C:/Program\ Files/Java/jdk1.7.0/include
  C:/Program\ Files\ \(x86\)/Java/jdk1.7.0/include
  C:/Program\ Files/Java/jdk1.8.0/include
) 

# possible paths for: 'jni_md.h'
list_paths(TRIAL_PATHS_INC_JNI_MD_7 "C:/Program\ Files/Java/jdk1.7.0_" "/include/win32" 200)
list_paths(TRIAL_PATHS_INC_JNI_MD_8 "C:/Program\ Files/Java/jdk1.8.0_" "/include/win32" 200)

set(TRIAL_PATHS_INC_JNI_MD
  ${TRIAL_PATHS_INC_JNI_MD_7}
  ${TRIAL_PATHS_INC_JNI_MD_8}
  C:/Program\ Files/Java/jdk1.7.0/include/win32
  C:/Program\ Files\ \(x86\)/Java/jdk1.7.0/include/win32
  C:/Program\ Files/Java/jdk1.8.0/include/win32
) 

# possible paths for: 'jvm.lib'
list_paths(TRIAL_PATHS_LIB_7 "C:/Program\ Files/Java/jdk1.7.0_" "/lib" 200)
list_paths(TRIAL_PATHS_LIB_8 "C:/Program\ Files/Java/jdk1.8.0_" "/lib" 200)

set(TRIAL_PATHS_LIB
  ${TRIAL_PATHS_LIB_7}
  ${TRIAL_PATHS_LIB_8}
  C:/Program\ Files/Java/jdk1.7.0/lib
  C:/Program\ Files\ \(x86\)/Java/jdk1.7.0/lib
  C:/Program\ Files/Java/jdk1.8.0/lib
) 

find_path(JNI_INCLUDE_JNI jni.h ${TRIAL_PATHS_INC_JNI})
find_path(JNI_INCLUDE_JNI_MD jni_md.h ${TRIAL_PATHS_INC_JNI_MD})

find_library(JNI_LIBRARIES jvm.lib ${TRIAL_PATHS_LIB})

set(JNI_INCLUDE_PATH ${JNI_INCLUDE_JNI} ${JNI_INCLUDE_JNI_MD})

## ----------------- ##


endif (UNIX)


# flag put to 1 if all required files are found
if (JNI_INCLUDE_PATH AND JNI_LIBRARIES)
  set(JNI_FOUND 1)
else ( )
  set(JNI_FOUND 0)
endif ( )

# outputs
mark_as_advanced(
  JNI_FOUND
  JNI_INCLUDE_PATH
  JNI_LIBRARIES 
)
