#
# Author: Nicolas Van der Noot & Timothee Habra
# September 2015
#
# Flags functions 
#

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                                   FLAGS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

## -- Release or debug -- ##

# use Release version (otherwise: Debug)
option (FLAG_RELEASE 
        "Release" ON)


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                                   MBSYSC SPECIFIC FLAGS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

function(mbsysc_specific_flags)  # flag for lib_mbsysC only (not set by project)

#    # run the simulation in real-time
#    option (FLAG_PRJ_FCT_PTR
#            "Call symbolic and user function via function pointer" OFF)

    ## -- Real-time features related -- ##

    # run the simulation in real-time
    option (FLAG_REAL_TIME
            "Real time" OFF)

    include(CMakeDependentOption)

    # use the SDL library to plot real-time graphs and to handle the user inputs (keyboard...)
    cmake_dependent_option(FLAG_PLOT "Enable realtime plots (need SDL)" ON "FLAG_REAL_TIME" OFF)

    # use the Java or OpenGL libraries to display in real-time the animation
    cmake_dependent_option(FLAG_VISU "Enable 3D visualization (need Java or OpenGL)" ON "FLAG_REAL_TIME" OFF)

    # use the Java library to display in real-time the animation
    cmake_dependent_option(FLAG_JAVA "Enable 3D visualization (need Java)" OFF "FLAG_VISU" OFF)

    # use the OpenGL library to display in real-time the animation
    cmake_dependent_option(FLAG_OPEN_GL "Enable 3D visualization (need OpenGL)" ON "FLAG_VISU" OFF)

endfunction()

## -- Static, dynamic libraries -- ##

## Compile Robotran MBSysC as shared lib (.so / .dll)
#option (FLAG_SHARED_LIB
#        "Compile as dynamic lib" OFF)



# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                         SET FLAG VALUE
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

function(set_flag arg0 arg1)
    set(${arg0} ${arg1} PARENT_SCOPE)
endfunction()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                    SAFETY: INCOMPATIBLE FLAGS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

function(flags_check)

    # real-time
    if (NOT FLAG_REAL_TIME)
        set( FLAG_PLOT OFF )
        set( FLAG_VISU OFF )
        set( FLAG_JAVA OFF )
        set( FLAG_OPEN_GL OFF )
    endif( )

    # 3D visu
    if (NOT FLAG_VISU)
        set( FLAG_JAVA OFF )
        set( FLAG_OPEN_GL OFF )
    endif( )

    # OpenGL only if no Java
    if (FLAG_JAVA)
        set( FLAG_OPEN_GL OFF )
    endif( )

    # separate build
    if (NOT FLAG_SEPARATE_BUILD)
        set( FLAG_SEPARATE_SYMBOLIC OFF )
        set( FLAG_SEPARATE_USER_FCT OFF )
    endif( )

    if ((FLAG_SEPARATE_BUILD) OR (NOT DEFINED FLAG_SEPARATE_BUILD))
        set ( FLAG_SHARED_LIB  ON )
        set ( FLAG_PRJ_FCT_PTR ON )
    else()
        set ( FLAG_SHARED_LIB  OFF )
        set ( FLAG_PRJ_FCT_PTR OFF )
    endif ( )

    # parent scope
    set( FLAG_RELEASE           ${FLAG_RELEASE}           PARENT_SCOPE )
    set( FLAG_REAL_TIME         ${FLAG_REAL_TIME}         PARENT_SCOPE )
    set( FLAG_PLOT              ${FLAG_PLOT}              PARENT_SCOPE )
    set( FLAG_VISU              ${FLAG_VISU}              PARENT_SCOPE )
    set( FLAG_JAVA              ${FLAG_JAVA}              PARENT_SCOPE )
    set( FLAG_OPEN_GL           ${FLAG_OPEN_GL}           PARENT_SCOPE )
    set( FLAG_SHARED_LIB        ${FLAG_SHARED_LIB}        PARENT_SCOPE )
    set( FLAG_SEPARATE_BUILD    ${FLAG_SEPARATE_BUILD}    PARENT_SCOPE )
    set( FLAG_SEPARATE_SYMBOLIC ${FLAG_SEPARATE_SYMBOLIC} PARENT_SCOPE )
    set( FLAG_SEPARATE_USER_FCT ${FLAG_SEPARATE_USER_FCT} PARENT_SCOPE )
    set( FLAG_PRJ_FCT_PTR       ${FLAG_PRJ_FCT_PTR}       PARENT_SCOPE )

endfunction()



# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#             CLEAN FLAG WHEN CHANGING SEPARATE BUILD OPTION
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


function(flags_clean)
    if ( FLAG_SEPARATE_BUILD )
        # FLAG REAL TIME is defined in the MBsysC CMakeList
        # By default, FLAG_REAL_TIME is defined at 1st run because FLAG_SEPARATE_BUILD=OFF by default
        unset(FLAG_REAL_TIME )
        unset(FLAG_PLOT CACHE)
        unset(FLAG_VISU CACHE)
        unset(FLAG_JAVA CACHE)
        unset(FLAG_OPEN_GL CACHE)
        unset(JNI_INCLUDE_JNI CACHE)
        unset(JNI_INCLUDE_JNI_MD CACHE)
        unset(SDL2_LIBRARIES_SDL2 CACHE)
        unset(FLAG_REAL_TIME CACHE)
    else()
        unset(LibRobotranC_DIR     CACHE)
        unset(LIB_MBSYSC_LOAD      CACHE)
        unset(LIB_MBSYSC_MODULES   CACHE)
        unset(LIB_MBSYSC_REALTIME  CACHE)
        unset(LIB_MBSYSC_UTILITIES CACHE)
    endif()
endfunction()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                       RELEASE OR DEBUG
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

function(release_debug)
    if (FLAG_RELEASE)
        set(CMAKE_BUILD_TYPE Release PARENT_SCOPE)
    else ( )
        set(CMAKE_BUILD_TYPE Debug PARENT_SCOPE)
    endif( )
endfunction()


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                       ADD DEFINITIONS
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

function(definitions)
    if(UNIX)
        add_definitions( -DUNIX )
        set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} -DUNIX)
    endif( )

    add_definitions( -DDIRDYNARED )
    set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} -DDIRDYNARED)

    if(FLAG_PRJ_FCT_PTR)
        add_definitions( -DPRJ_FCT_PTR )
        set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} -DPRJ_FCT_PTR)
    endif( )

    if(FLAG_REAL_TIME)
        add_definitions( -DREAL_TIME )
        set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} -DREAL_TIME)
    endif( )

    if(FLAG_PLOT)
        add_definitions( -DSDL )
        set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} -DSDL)
    endif( )

    if(FLAG_VISU)
        add_definitions( -DVISU_3D )
        set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} -DVISU_3D)
    endif( )

    if(FLAG_JAVA)
        add_definitions( -DJAVA )
        set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} -DJAVA)
    endif( )

    if(FLAG_OPEN_GL)
        add_definitions( -DOPEN_GL )
        set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} -DOPEN_GL)
    endif( )

    set(LIB_MBSYSC_DEFINITIONS ${LIB_MBSYSC_DEFINITIONS} PARENT_SCOPE)
endfunction()
