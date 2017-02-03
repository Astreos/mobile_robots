#
# Author: Nicolas Van der Noot
# September 2015
#
# Library functions 
#

# Libxml2
function(xml_lib arg0)
    add_definitions( -DXML )
    if (WIN32)
        find_package(LibXml2Win REQUIRED)
    else ( )
        find_package(LibXml2 REQUIRED)
    endif ( )
    set(LIBXML2_INCLUDE_PATH ${LIBXML2_INCLUDE_DIR})
    if (LIBXML2_FOUND)
        include_directories (${LIBXML2_INCLUDE_PATH})   
        message( "\n  >> ${arg0}: Libxml2 found")
    else ( )
        message( "\n  >> ${arg0}: Libxml2 not found; for Windows, modify ${ROBOTRAN_SOURCE_DIR}/conf/FindLibXml2Win.cmake")
    endif ( )

    message("      -> INC : ${LIBXML2_INCLUDE_PATH}")
    message("      -> LIB : ${LIBXML2_LIBRARIES}\n")

    set(LIBXML2_INCLUDE_PATH ${LIBXML2_INCLUDE_PATH} PARENT_SCOPE)
    set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES} PARENT_SCOPE)
      
endfunction()

# GSL
function(gsl_lib arg0)
    find_package(GSL REQUIRED)
    if (GSL_FOUND)
        include_directories (${GSL_INCLUDE_DIR})
        message( "\n  >> ${arg0}: GSL found")
    else ( )
        message( "\n  >> ${arg0}: GSL not found, modify ${ROBOTRAN_SOURCE_DIR}/conf/FindGSL.cmake")
    endif ( )

    message("      -> INC : ${GSL_INCLUDE_DIR}")
    message("      -> LIB : ${GSL_LIBRARIES}\n")

    set(GSL_LIBRARIES ${GSL_LIBRARIES} PARENT_SCOPE)
endfunction()

# SDL
function(sdl_lib arg0)
    find_package(SDL2 REQUIRED)
    if (SDL2_FOUND)
        include_directories (${SDL2_INCLUDE_PATH})
        message( "\n  >> ${arg0}: SDL 2.0 found")        
    else ( )
        message( "\n  >> ${arg0}: SDL 2.0 not found, modify ${ROBOTRAN_SOURCE_DIR}/conf/FindSDL2.cmake")        
    endif ( )

    message("      -> INC : ${SDL2_INCLUDE_PATH}")
    message("      -> LIB : ${SDL2_LIBRARIES}\n")

    # SDL_ttf
    find_package(SDL2_ttf REQUIRED)
    if (SDL2TTF_FOUND)              
        include_directories (${SDL2TTF_INCLUDE_PATH})
        message( "\n  >> ${arg0}: SDL 2.0 ttf found")                
    else ( )
        message( "\n  >> ${arg0}: SDL 2.0 ttf not found, modify ${ROBOTRAN_SOURCE_DIR}/conf/FindSDL2_ttf.cmake")
    endif ( )

    message("      -> INC : ${SDL2TTF_INCLUDE_PATH}")
    message("      -> LIB : ${SDL2TTF_LIBRARIES}\n")

    set(SDL2_LIBRARIES ${SDL2_LIBRARIES} PARENT_SCOPE)
    set(SDL2TTF_LIBRARIES ${SDL2TTF_LIBRARIES} PARENT_SCOPE)
endfunction()

# SDL header
function(sdl_header_lib arg0)
    set ( CMAKE_MODULE_PATH ${ROBOTRAN_SOURCE_DIR}/conf )
    find_package(SDL2 REQUIRED)
    if (SDL2_FOUND)
        include_directories (${SDL2_INCLUDE_PATH})
    else ( )
        message( "\n  >> ${arg0}: SDL 2.0 not found, modify ${ROBOTRAN_SOURCE_DIR}/conf/FindSDL2.cmake")
    endif ( )
endfunction()

# Java
function(java_lib arg0)
    find_package(JNI REQUIRED)
    if (JNI_FOUND)
        include_directories (${JNI_INCLUDE_PATH})
        message( "\n  >> ${arg0}: JNI found")
    else ( )
        message( "\n  >> ${arg0}: JNI not found, modify ${ROBOTRAN_SOURCE_DIR}/conf/FindJNI.cmake")
    endif ( )

    message("      -> INC : ${JNI_INCLUDE_PATH}")
    message("      -> LIB : ${JNI_LIBRARIES}\n")

    # Java 3D
    find_package(J3D REQUIRED)
    if (J3D_FOUND)      
        message( "\n  >> ${arg0}: Java 3D found")
    else ( )
        message( "\n  >> ${arg0}: Java 3D not found, modify ${ROBOTRAN_SOURCE_DIR}/conf/FindJ3D.cmake")
    endif ( )

    message("      -> PATH : ${J3D_PATH}\n")

    set(JNI_LIBRARIES ${JNI_LIBRARIES} PARENT_SCOPE)
    set(J3D_PATH ${J3D_PATH} PARENT_SCOPE)
endfunction()

# OpenGL
function(open_gl_lib arg0)
    add_definitions( -DOPEN_GL )
    find_package(Opengl REQUIRED)

    if(OPEN_GL_FOUND)
        include_directories (${OPEN_GL_INCLUDE_PATH})
        message( "\n  >> ${arg0}: OpenGL found")
    else ( )
        message( "\n  >> ${arg0}: OpenGL not found, modify ${ROBOTRAN_SOURCE_DIR}/conf/FindOpengl.cmake")
    endif ( )

    message("      -> INC : ${OPEN_GL_INCLUDE_PATH}")
    message("      -> LIB : ${OPEN_GL_LIBS}\n")

    set(OPEN_GL_INCLUDE_PATH ${OPEN_GL_INCLUDE_PATH} PARENT_SCOPE)
    set(OPEN_GL_LIBS ${OPEN_GL_LIBS} PARENT_SCOPE)

endfunction()

# OpenGL
function(p_threads_lib arg0)
    find_package (Threads)

    message( "\n  >> ${arg0}: Threads lib")
    message("      -> LIB : ${CMAKE_THREAD_LIBS_INIT}\n")

    set(CMAKE_THREAD_LIBS_INIT ${CMAKE_THREAD_LIBS_INIT} PARENT_SCOPE)

endfunction()
