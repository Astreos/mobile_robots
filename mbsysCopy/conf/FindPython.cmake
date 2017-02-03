
### looking for python ###
find_package(PythonInterp REQUIRED)


# looking for prefix of python installation
exec_program( ${PYTHON_EXECUTABLE}
              ARGS "-c \"import sys; print(sys.prefix)\""
              OUTPUT_VARIABLE PYTHON_PREFIX
             )
# looking for version of installed python
exec_program( ${PYTHON_EXECUTABLE}
              ARGS "-c \"import sys; print(str(sys.version_info[0])+'.'+str(sys.version_info[1]))\""
              OUTPUT_VARIABLE PYTHON_VERSION
             )

# looking for python include dir
exec_program( ${PYTHON_EXECUTABLE}
              ARGS "-c \"import distutils.sysconfig; print(distutils.sysconfig.get_python_inc())\""
              OUTPUT_VARIABLE PYTHON_INCLUDE_DIR
             )

# this block has been taken from siconos FindPythonFull.cmake
# it looks for the python library from the prefix and version of the interpretor previously found
string(REGEX REPLACE "([0-9]+).([0-9]+)" "\\1\\2" PYTHON_VERSION_NO_DOTS ${PYTHON_VERSION})
# --- Search python library corresponding to python exec.
find_library(PYTHON_LIBRARY NAMES python${PYTHON_VERSION_NO_DOTS} python${PYTHON_VERSION}
             NO_DEFAULT_PATH
             HINTS ${PYTHON_PREFIX}
                   ${PYTHON_PREFIX}/lib/python${PYTHON_VERSION}/config
                   ${PYTHON_PREFIX}/lib/python${PYTHON_VERSION}/config-${CMAKE_LIBRARY_ARCHITECTURE}
             PATH_SUFFIXES lib libs
            )

set(PYTHON_LIBRARIES ${PYTHON_LIBRARY} CACHE FILEPATH "Python libraries" FORCE)
set(PYTHON_INCLUDE_DIR ${PYTHON_INCLUDE_DIR} CACHE FILEPATH "Path to Python.h" FORCE)

message(STATUS "Found Python executable: ${PYTHON_EXECUTABLE}")
message(STATUS "Found Python library: ${PYTHON_LIBRARIES}")
message(STATUS "Python version is : ${PYTHON_VERSION_STRING}")
message(STATUS "Python include dir is : ${PYTHON_INCLUDE_DIR}")


### looking for numpy ###
if (NUMPY_INCLUDE_DIR)
  # in cache already
  set (NUMPY_FIND_QUIETLY TRUE)
endif (NUMPY_INCLUDE_DIR)

exec_program(${PYTHON_EXECUTABLE}
             ARGS "-c \"import numpy; print(numpy.get_include())\""
             OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
             RETURN_VALUE NUMPY_NOT_FOUND
            )

if (NUMPY_NOT_FOUND)
  if(Numpy_FIND_REQUIRED)
    message (FATAL_ERROR "Numpy package/header is missing")
  endif (Numpy_FIND_REQUIRED)
else (NUMPY_NOT_FOUND)
  if (NOT NUMPY_FIND_QUIETLY)
    message (STATUS "Numpy headers found: ${NUMPY_INCLUDE_DIR}")
  endif (NOT NUMPY_FIND_QUIETLY)
endif (NUMPY_NOT_FOUND)

if (NUMPY_INCLUDE_DIR)
  set (NUMPY_FOUND TRUE)
  set (NUMPY_INCLUDE_DIR ${NUMPY_INCLUDE_DIR} CACHE STRING "Numpy include path")
else (NUMPY_INCLUDE_DIR)
  set(NUMPY_FOUND FALSE)
endif (NUMPY_INCLUDE_DIR)

MARK_AS_ADVANCED (NUMPY_INCLUDE_DIR)

### looking for cython ###
find_program(CYTHON_EXECUTABLE cython)

