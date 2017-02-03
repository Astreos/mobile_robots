#
# author: Nicolas Van der Noot
#
# This file finds the include folders (header files) and the libraries
# related to OpenGL and the other required components
#
# OPEN_GL_FOUND :       1 if all required files found (0 otherwise)
# OPEN_GL_INCLUDE_PATH : include paths (for the header files) -> for compilation
# OPEN_GL_LIBS :         libraries -> for linkage
#

if (UNIX)

if (APPLE)
  

## ---- MAC OS ---- ##

# possible paths for: 'gl.h'
set(TRIAL_PATHS_GL
  /System/Library/Frameworks/OpenGL.framework/Versions/A
)

# possible paths for: 'glm.hpp'
set(TRIAL_PATHS_GLM
  /usr/local/include/glm/
)

# possible paths for: 'glew.h'
set(TRIAL_PATHS_GLEW
  /usr/local/include/GL
)

# possible paths for: 'glfw3.h'
set(TRIAL_PATHS_GLFW3
  /usr/local/include/GLFW
)

# possible paths for: 'libGL.dylib'
set(TRIAL_PATHS_LIB_OPEN_GL
  /System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries
)

# possible paths for: 'libglfw3.a'
set(TRIAL_PATHS_LIB_GLFW
  /usr/local/lib
)

# possible paths for: 'libGLEW.dylib'
set(TRIAL_PATHS_LIB_GLEW
  /usr/local/lib
)

find_path(PATH_GL    gl.h    ${TRIAL_PATHS_GL}   )
find_path(PATH_GLM   glm.hpp ${TRIAL_PATHS_GLM}  )
find_path(PATH_GLEW  glew.h  ${TRIAL_PATHS_GLEW} )
find_path(PATH_GLFW3 glfw3.h ${TRIAL_PATHS_GLFW3})

find_library(PATH_LIB_OPEN_GL libGL.dylib   ${TRIAL_PATHS_LIB_OPEN_GL})
find_library(PATH_LIB_GLFW    libglfw3.a    ${TRIAL_PATHS_LIB_GLFW}   )
find_library(PATH_LIB_GLEW    libGLEW.dylib ${TRIAL_PATHS_LIB_GLEW}   )

set(OPEN_GL_INCLUDE_PATH ${PATH_GL} ${PATH_GLM} ${PATH_GLEW} ${PATH_GLFW3})
set(OPEN_GL_LIBS ${PATH_LIB_OPEN_GL} ${PATH_LIB_GLFW} ${PATH_LIB_GLEW} "-framework Cocoa" "-framework OpenGL" "-framework CoreVideo" "-framework IOKit")


## ---------------- ##


else (APPLE)
  

## ---- LINUX ---- ##

# main OpenGL libraries
find_package(OpenGL REQUIRED)

# possible paths for: 'glm.hpp'
set(TRIAL_PATHS_GLM
  /usr/include/glm
)

# possible paths for: 'glew.h'
set(TRIAL_PATHS_GLEW
  /usr/include/GL
)

# possible paths for: 'glfw3.h'
set(TRIAL_PATHS_GLFW3
  /usr/include/GLFW
  /usr/local/include/GLFW
)

# possible paths for: 'libglfw.so'
set(TRIAL_PATHS_LIB_GLFW
  /usr/lib/x86_64-linux-gnu
  /usr/local/lib
)

# possible paths for: 'libGLEW.so'
set(TRIAL_PATHS_LIB_GLEW
  /usr/lib/x86_64-linux-gnu
  /usr/local/lib
)

find_path(PATH_GLM   glm.hpp ${TRIAL_PATHS_GLM}  )
find_path(PATH_GLEW  glew.h  ${TRIAL_PATHS_GLEW} )
find_path(PATH_GLFW3 glfw3.h ${TRIAL_PATHS_GLFW3})

find_library(PATH_LIB_GLFW libglfw.so ${TRIAL_PATHS_LIB_GLFW})
find_library(PATH_LIB_GLEW libGLEW.so ${TRIAL_PATHS_LIB_GLEW})

set(OPEN_GL_INCLUDE_PATH ${OPENGL_INCLUDE_DIR} ${PATH_GLM} ${PATH_GLEW} ${PATH_GLFW3})
set(OPEN_GL_LIBS ${OPENGL_LIBRARIES} ${PATH_LIB_GLFW} ${PATH_LIB_GLEW})

## --------------- ##


endif (APPLE)

else (UNIX)


## ---- WINDOWS ---- ##

# possible paths for: 'glm.hpp'
set(TRIAL_PATHS_GLM
  ${ROBOTRAN_SOURCE_DIR}/${WIN_LIB_DIRECTORY}/include/glm
)

# possible paths for: 'glew.h'
set(TRIAL_PATHS_GLEW
  ${ROBOTRAN_SOURCE_DIR}/${WIN_LIB_DIRECTORY}/include/GL
)

# possible paths for: 'glfw3.h'
set(TRIAL_PATHS_GLFW3
  ${ROBOTRAN_SOURCE_DIR}/${WIN_LIB_DIRECTORY}/include/GLFW
)

# possible paths for: 'OpenGL32.lib'
set(TRIAL_PATHS_LIB_OPEN_GL
  C:/Program\ Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x64
  C:/Program\ Files\ \(x86\)/Windows\ Kits/8.1/Lib/winv6.3/um/x86
)

# possible paths for: 'glfw3.lib'
set(TRIAL_PATHS_LIB_GLFW
  ${ROBOTRAN_SOURCE_DIR}/${WIN_LIB_DIRECTORY}/lib/GLFW/${MSVC_VERSION}
)

# possible paths for: 'glew32.lib'
set(TRIAL_PATHS_LIB_GLEW
  ${ROBOTRAN_SOURCE_DIR}/${WIN_LIB_DIRECTORY}/lib/GL
)

find_path(PATH_GLM   glm.hpp ${TRIAL_PATHS_GLM}  )
find_path(PATH_GLEW  glew.h  ${TRIAL_PATHS_GLEW} )
find_path(PATH_GLFW3 glfw3.h ${TRIAL_PATHS_GLFW3})

find_library(PATH_LIB_GLFW glfw3.lib ${TRIAL_PATHS_LIB_GLFW})
find_library(PATH_LIB_GLEW glew32.lib ${TRIAL_PATHS_LIB_GLEW})
find_library(PATH_LIB_OPEN_GL OpenGL32.lib ${TRIAL_PATHS_LIB_OPEN_GL})

set(OPEN_GL_INCLUDE_PATH ${PATH_GLM} ${PATH_GLEW} ${PATH_GLFW3})
set(OPEN_GL_LIBS ${PATH_LIB_OPEN_GL} ${PATH_LIB_GLFW} ${PATH_LIB_GLEW})

## ----------------- ##


endif (UNIX)


# flag put to 1 if all required files are found
if (OPEN_GL_INCLUDE_PATH AND OPEN_GL_LIBS)
  set(OPEN_GL_FOUND 1)
else ( )
  set(OPEN_GL_FOUND 0)
endif ( )

# outputs
mark_as_advanced(
  OPEN_GL_FOUND
  OPEN_GL_INCLUDE_PATH
  OPEN_GL_LIBS 
)
