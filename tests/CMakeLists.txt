# catch
include("${CMAKE_CURRENT_SOURCE_DIR}/catch2/cmake/Catch.cmake")

# test ThawScape
file(GLOB TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/test*.cpp)
add_executable(ThawScapeTests ${TEST_SRC})
target_link_libraries(ThawScapeTests ThawScapeLib)
catch_discover_tests(ThawScapeTests)

# standalone apps for testing
add_executable(run_mfd run_mfd.cpp)
target_link_libraries(run_mfd ThawScapeLib)

# copy test input files
configure_file(ThawScapeTestInit.ini ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(test_raster.asc ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# run test of full code running
add_test(
    NAME ThawScapeFullRun
    COMMAND ${CMAKE_COMMAND}
        -DTEST_RUN_DIR=${CMAKE_CURRENT_BINARY_DIR}/ThawScapeFullRun
        -DTEST_SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -DTEST_REF_DIR=${CMAKE_CURRENT_SOURCE_DIR}/full-run-ref
        -DTEST_BINARY=$<TARGET_FILE:ThawScape>
        -P ${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake
)
