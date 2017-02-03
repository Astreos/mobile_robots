#
# Author: Nicolas Van der Noot
#
# Make options functions 
#

# Make options
function(make_options)

    if (UNIX)

        # general variables
        set( SCRIPTS_PATH ${ROBOTRAN_SOURCE_DIR}/cmake_aux/scripts )

        # scripts executable
        execute_process(COMMAND chmod +x ${SCRIPTS_PATH}/mbsysc_build)
        execute_process(COMMAND chmod +x ${SCRIPTS_PATH}/symb_build)
        execute_process(COMMAND chmod +x ${SCRIPTS_PATH}/userfct_build)
        execute_process(COMMAND chmod +x ${SCRIPTS_PATH}/make_options)

        # -- List options -- #

        # make_options (options listing with an example of cmake script)
        add_custom_target(make_options COMMAND ${CMAKE_COMMAND} -P ${SCRIPTS_PATH}/make_options )


        # -- Make mbs_common libraries -- #

        # mbsysc_basic (SHARED_LIB, SEPARATE_BUILD, RELEASE, REAL_TIME, PLOT, VISU)
        add_custom_target(mbsysc_basic COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./mbsysc_build ${ROBOTRAN_SOURCE_DIR} ON ON ON OFF OFF OFF OFF OFF)

        # mbsysc_plot (SHARED_LIB, SEPARATE_BUILD, RELEASE, REAL_TIME, PLOT, VISU)
        add_custom_target(mbsysc_plot COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./mbsysc_build ${ROBOTRAN_SOURCE_DIR} ON ON ON ON ON OFF OFF OFF)

        # mbsysc_visu (SHARED_LIB, SEPARATE_BUILD, RELEASE, REAL_TIME, PLOT, VISU)
        add_custom_target(mbsysc_visu COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./mbsysc_build ${ROBOTRAN_SOURCE_DIR} ON ON ON ON OFF ON OFF ON)

        # mbsysc_realtime (SHARED_LIB, SEPARATE_BUILD, RELEASE, REAL_TIME, PLOT, VISU)
        add_custom_target(mbsysc_realtime COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./mbsysc_build ${ROBOTRAN_SOURCE_DIR} ON ON ON ON ON ON OFF ON)

        # mbsysc_clean (SHARED_LIB, SEPARATE_BUILD, RELEASE, REAL_TIME, PLOT, VISU)
        add_custom_target(mbsysc_clean COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./mbsysc_build ${ROBOTRAN_SOURCE_DIR} "clean")


        # -- Make user functions libraries -- #

        # userfct_basic (REAL_TIME, PLOT, VISU)
        add_custom_target(userfct_basic COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./userfct_build ${ROBOTRAN_SOURCE_DIR} ${PROJECT_SOURCE_DIR} OFF OFF OFF)

        # userfct_plot (REAL_TIME, PLOT, VISU)
        add_custom_target(userfct_plot COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./userfct_build ${ROBOTRAN_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ON ON OFF)

        # userfct_visu (REAL_TIME, PLOT, VISU)
        add_custom_target(userfct_visu COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./userfct_build ${ROBOTRAN_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ON OFF ON)

        # userfct_realtime (REAL_TIME, PLOT, VISU)
        add_custom_target(userfct_realtime COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./userfct_build ${ROBOTRAN_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ON ON ON)

        # userfct_clean (REAL_TIME, PLOT, VISU)
        add_custom_target(userfct_clean COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./userfct_build ${ROBOTRAN_SOURCE_DIR} ${PROJECT_SOURCE_DIR} "clean")


        # -- Make symbolic library -- #

        # symb_lib
        add_custom_target(symb_lib COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./symb_build ${ROBOTRAN_SOURCE_DIR} ${PROJECT_SOURCE_DIR})

        # symb_clean
        add_custom_target(symb_clean COMMAND ${CMAKE_COMMAND} -E chdir ${SCRIPTS_PATH} ./symb_build ${ROBOTRAN_SOURCE_DIR} ${PROJECT_SOURCE_DIR} "clean")

    endif ( )

endfunction()
