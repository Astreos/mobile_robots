#
# author: Nicolas Van der Noot
# Feb 27 2014
#
# This file finds the include folders (header files) and the libraries
# related to SDL2_ttf
#
# SDL2TTF_FOUND :        1 if all required files found (0 otherwise)
# SDL2TTF_INCLUDE_PATH : include paths (for the header files) -> for compilation
# SDL2TTF_LIBRARIES :    libraries -> for linkage
#


if (UNIX)

if (APPLE)
	

## ---- MAC OS ---- ##

# possible paths for: 'SDL_ttf.h'
set(TRIAL_PATHS_INC
  /Library/Frameworks/SDL2_ttf.framework/Versions/A/Headers
  /usr/local/include/SDL2
) 

# possible paths for: 'libSDL2_ttf.dylib'
set(TRIAL_PATHS_LIB
  /usr/local/lib
) 

find_path(SDL2TTF_INCLUDE_PATH SDL_ttf.h ${TRIAL_PATHS_INC})

find_library (SDL2TTF_LIBRARIES SDL2_ttf)

if (NOT SDL2TTF_LIBRARIES)
  find_library(SDL2TTF_LIBRARIES libSDL2_ttf.dylib ${TRIAL_PATHS_LIB})
endif ( )

## ---------------- ##


else (APPLE)
	

## ---- LINUX ---- ##	

# possible paths for: 'SDL_ttf.h'
set(TRIAL_PATHS_INC
  /usr/local/include/SDL2
  /usr/include/SDL2
) 

# possible paths for: 'libSDL2_ttf.so'
set(TRIAL_PATHS_LIB
  /usr/local/lib
  /usr/lib/x86_64-linux-gnu
) 

find_path(SDL2TTF_INCLUDE_PATH SDL_ttf.h ${TRIAL_PATHS_INC})

find_library(SDL2TTF_LIBRARIES libSDL2_ttf.so ${TRIAL_PATHS_LIB})

## --------------- ##


endif (APPLE)

else (UNIX)


## ---- WINDOWS ---- ##

# possible paths for: 'SDL_ttf.h'
set(TRIAL_PATHS_INC
  ${PROJECT_SOURCE_DIR}/src/other/${WIN_LIB_DIRECTORY}/include/SDL
  ${PROJECT_ABS_PATH}/src/other/${WIN_LIB_DIRECTORY}/include/SDL
  ${ROBOTRAN_SOURCE_DIR}/${WIN_LIB_DIRECTORY}/include/SDL
) 

# possible paths for: 'SDL2_ttf.lib'
set(TRIAL_PATHS_LIB
  ${PROJECT_SOURCE_DIR}/src/other/${WIN_LIB_DIRECTORY}/lib
  ${PROJECT_ABS_PATH}/src/other/${WIN_LIB_DIRECTORY}/lib
  ${ROBOTRAN_SOURCE_DIR}/${WIN_LIB_DIRECTORY}/lib
) 

find_path(SDL2TTF_INCLUDE_PATH SDL_ttf.h ${TRIAL_PATHS_INC})

find_library(SDL2TTF_LIBRARIES SDL2_ttf.lib ${TRIAL_PATHS_LIB})

## ----------------- ##


endif (UNIX)


# flag put to 1 if all required files are found
if (SDL2TTF_INCLUDE_PATH AND SDL2TTF_LIBRARIES)
  set(SDL2TTF_FOUND 1)
else ( )
  set(SDL2TTF_FOUND 0)
endif ( )

# outputs
mark_as_advanced(
  SDL2TTF_FOUND
  SDL2TTF_INCLUDE_PATH
  SDL2TTF_LIBRARIES 
)
