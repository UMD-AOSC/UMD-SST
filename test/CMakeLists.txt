list( APPEND umdsst_test_input
  testinput/errorcovariance.yml
  testinput/geometry.yml
  testinput/getvalues.yml
  testinput/hofx3d.yml
  testinput/increment.yml
  testinput/lineargetvalues.yml
  testinput/linearvarchange_stddev.yml
  testinput/modelaux.yml
  testinput/state.yml
  testinput/dirac.yml
  testinput/staticbinit.yml
  testinput/var.yml
  )

list( APPEND umdsst_test_ref
  testref/hofx3d.ref
  testref/dirac.ref
  testref/staticbinit.ref
  testref/var.ref
  )

list( APPEND umdsst_test_data
  Data/19850101_regridded_sst_1x1.nc
  Data/landmask_1x1.nc
  Data/obs_sst.nc
  Data/rossby_radius.dat
  )


# Create directory for test input/output/reference and symlink all files
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testinput)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testoutput)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testref)
foreach(FILENAME ${umdsst_test_input} ${umdsst_test_ref})
    execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink
           ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
           ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME} )
endforeach(FILENAME)

# create static data directory, and symlink all files.
execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink
          ${CMAKE_CURRENT_SOURCE_DIR}/Data
          ${CMAKE_CURRENT_BINARY_DIR}/data_static )
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data_generated)


# number of PEs to use for MPI enabled tests.
set( UMDSST_TESTS_MAX_MPI 1)


#---------------------------------------------------------------------------------------------------
# The following is a wrapper to simplify the generation of tests of the umdsst
# executables. There are two types of tests:
#  1) umdsst executables (EXE must be given)
#  2) interface tests  (SRC must be given)
#
# Arguments:
#  NAME      - the name of the test (after test_umdsst is prepended), as well
#              as the base name of the input confg file
#  EXE       - name of executable to be used.
#  SRC       - The name of the unit test source file to compile
#  CFG       - The name of the yaml file, if the default testinput/${NAME}.yaml
#              is not to be used
#  MPI       - The number of MPI PEs to use. If not specified, ${UMDSST_TESTS_MAX_MPI}
#              will be used
# TEST_DEPENDS - list of tests this test depends on
#---------------------------------------------------------------------------------------------------
function(umdsst_add_test)
  # parse the passed arguments
  set(prefix     ARG)
  set(novals     NOCOMPARE NOTRAPFPE)
  set(singlevals NAME EXE SRC CFG MPI CLEAN)
  set(multivals  TEST_DEPENDS)
  cmake_parse_arguments(${prefix}
                        "${novals}" "${singlevals}" "${multivals}"
                        ${ARGN})

  # determine if floating point error trapping should be set
  if( ARG_NOTRAPFPE)
    set ( TRAPFPE_ENV "OOPS_TRAPFPE=0")
  else()
    set ( TRAPFPE_ENV "OOPS_TRAPFPE=1")
  endif()

  # determine the default config file name
  if ( ARG_CFG )
    set ( CONFIG_FILE testinput/${ARG_CFG} )
  else()
    set ( CONFIG_FILE testinput/${ARG_NAME}.yml )
  endif()


  # MPI PEs
  set( MPI ${UMDSST_TESTS_MAX_MPI} )
  if ( ARG_MPI )
    set( MPI ${ARG_MPI})
  endif()

  # # find the MPI command
  # set(MPI_CMD "${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPI_PES}")

  # setup working directory
  set( WORKDIR ${CMAKE_CURRENT_BINARY_DIR}/test_workdir/${ARG_NAME})
  file( MAKE_DIRECTORY ${WORKDIR} )
  foreach(DIRNAME data_generated data_static testinput testoutput testref)
    execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_CURRENT_BINARY_DIR}/${DIRNAME}
      ${WORKDIR}/${DIRNAME} )
  endforeach()

  # setup output directory
  file( MAKE_DIRECTORY ${WORKDIR}/data_generated/${ARG_NAME})

  # Are we building a unit test / or running a soca executable?
  if ( ARG_SRC )
    # building a unit test
    ecbuild_add_test( TARGET  test_umdsst_${ARG_NAME}
                      SOURCES executables/${ARG_SRC}
                      ARGS    ${CONFIG_FILE}
                      LIBS    umdsst
                      MPI     ${MPI}
                      ENVIRONMENT ${TRAPFPE_ENV}
                      WORKING_DIRECTORY ${WORKDIR}
                      TEST_DEPENDS ${ARG_TEST_DEPENDS})
    set( EXE ./test_umdsst_${ARG_NAME})

  else()
    # running a umdsst executable.
    set( EXE ${CMAKE_BINARY_DIR}/bin/${ARG_EXE})
    ecbuild_add_test( TARGET  test_umdsst_${ARG_NAME}
                      MPI ${MPI}
                      COMMAND ${EXE}
                      WORKING_DIRECTORY ${WORKDIR}
                      ARGS ${CONFIG_FILE}
                      ENVIRONMENT
                              ${TRAPFPE_ENV}
                      DEPENDS ${ARG_EXE}
                      TEST_DEPENDS ${ARG_TEST_DEPENDS})
  endif()
endfunction()

#================================================================================
# Tests of class interfaces
# TODO: uncomment all of these tests as the appropriate interfaces are implemented
#================================================================================

umdsst_add_test( NAME geometry
                 SRC TestGeometry.cc )

umdsst_add_test( NAME state
                 SRC TestState.cc )
# ecbuild_add_test(
#   TARGET  test_umdsst_increment
#   SOURCES executables/TestIncrement.cc
#   ARGS    testinput/increment.yml
#   MPI     ${MPI_PES}
#   LIBS    umdsst )

#  ecbuild_add_test(
#    TARGET  test_umdsst_modelauxcontrol
#    SOURCES executables/TestModelAuxControl.cc
#    ARGS    testinput/modelaux.yml
#    MPI     ${MPI_PES}
#    LIBS    umdsst )

#    ecbuild_add_test(
#      TARGET  test_umdsst_lineargetvalues
#      SOURCES executables/TestLinearGetValues.cc
#      ARGS    testinput/lineargetvalues.yml
#      MPI     ${MPI_PES}
#      LIBS    umdsst )

#    ecbuild_add_test(
#     TARGET  test_umdsst_linearvarchange_stddev
#     SOURCES executables/TestLinearVariableChange.cc
#     ARGS    testinput/linearvarchange_stddev.yml
#     MPI     ${MPI_PES}
#     LIBS    umdsst )

# ecbuild_add_test(
#   TARGET  test_umdsst_errorcovariance
#   SOURCES executables/TestErrorCovariance.cc
#   ARGS    testinput/errorcovariance.yml
#   MPI     ${MPI_PES}
#   LIBS    umdsst
#   TEST_DEPENDS test_umdsst_staticbinit)

#  ecbuild_add_test(
#    TARGET  test_umdsst_modelauxcovariance
#    SOURCES executables/TestModelAuxCovariance.cc
#    ARGS    testinput/modelaux.yml
#    MPI     ${MPI_PES}
#    LIBS    umdsst )

#  ecbuild_add_test(
#    TARGET  test_umdsst_modelauxincrement
#    SOURCES executables/TestModelAuxIncrement.cc
#    ARGS    testinput/modelaux.yml
#    MPI     ${MPI_PES}
#    LIBS    umdsst )


#================================================================================
# Test of executables
#================================================================================

# umdsst_exe_test( NAME hofx3d
#                   EXE  umdsst_hofx3d.x )

# umdsst_exe_test( NAME staticbinit
#                   EXE  umdsst_staticbinit.x
#                   CLEAN Data/bump/* )

# umdsst_exe_test( NAME dirac
#                   EXE  umdsst_dirac.x
#                   TEST_DEPENDS test_umdsst_staticbinit )

# umdsst_exe_test( NAME var
#                   EXE  umdsst_var.x
#                   TEST_DEPENDS test_umdsst_staticbinit )
