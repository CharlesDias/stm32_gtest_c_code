include(GoogleTest)

############################ Test Project ############################
set(SOURCE_FOLDER_PATH "${CMAKE_SOURCE_DIR}/stm32-cube-ide")
set(TEST_FOLDER_PATH "${CMAKE_SOURCE_DIR}/tests")

# target_include_directories( ${HELLO_UNIT_TEST} PRIVATE
#    ${TEST_FOLDER_PATH}/header-overrides
#    ${TEST_FOLDER_PATH}/mock
# )

include_directories(
   ${TEST_FOLDER_PATH}/header-overrides
   ${TEST_FOLDER_PATH}/mock
)

set(ASSERT_MOCK AssertMock)
add_library(${ASSERT_MOCK}
   ${TEST_FOLDER_PATH}/mock/assert_mock.c
) 

######################################################################
# Hello unit test
set(HELLO_UNIT_TEST HelloUnitTest)
add_executable( ${HELLO_UNIT_TEST}
   ${TEST_FOLDER_PATH}/unit/hello_unit_test.cpp
)

target_link_libraries( ${HELLO_UNIT_TEST} PUBLIC
   GTest::gmock_main
)
gtest_discover_tests( ${HELLO_UNIT_TEST}
)

######################################################################
# Hello integration test
set(HELLO_INTEGRATION_TEST HelloIntegrationTest)
add_executable( ${HELLO_INTEGRATION_TEST}
${TEST_FOLDER_PATH}/integration/hello_integration_test.cpp
)
target_link_libraries( ${HELLO_INTEGRATION_TEST}
   GTest::gmock_main
)
gtest_discover_tests( ${HELLO_INTEGRATION_TEST}
)

######################################################################
# Unit tests for drivers functions
set(DRIVERS_UNIT_TEST DriversUnitTest)
add_executable( ${DRIVERS_UNIT_TEST}
   ${TEST_FOLDER_PATH}/unit/gpio_test.cpp
)

target_link_libraries( ${DRIVERS_UNIT_TEST} PUBLIC
   ${DRIVERS_LIBRARY_TARGET}
   # ${ASSERT_MOCK}
   GTest::gmock_main
)
gtest_discover_tests( ${DRIVERS_UNIT_TEST}
)

######################################################################
# Unit tests for middleware functions
set(MIDDLEWARE_UNIT_TEST MiddlewareUnitTest)
add_executable( ${MIDDLEWARE_UNIT_TEST}
   ${TEST_FOLDER_PATH}/unit/led_test.cpp
)

target_link_libraries( ${MIDDLEWARE_UNIT_TEST} PUBLIC
   ${MIDDLEWARE_LIBRARY_TARGET}
   GTest::gmock_main
)
gtest_discover_tests( ${MIDDLEWARE_UNIT_TEST}
)

######################################################################
# Settings for code coverage
if (ENABLE_COVERAGE)
   set(COVERAGE_MAIN "coverage")

   set(COVERAGE_EXCLUDES
      "${PROJECT_SOURCE_DIR}/build/*"
      "${PROJECT_SOURCE_DIR}/stm32-cube-ide/*"
      "${PROJECT_SOURCE_DIR}/cmake/*"
      "${PROJECT_SOURCE_DIR}/docs/*"
      # "${PROJECT_SOURCE_DIR}/external/*"
      "${PROJECT_SOURCE_DIR}/tests/*"
      "/usr/include/*"
   )

   set(COVERAGE_DEPENDENCIES 
      ${DRIVERS_UNIT_TEST}
      ${MIDDLEWARE_UNIT_TEST}
      ${HELLO_UNIT_TEST}
      ${HELLO_INTEGRATION_TEST}
   ) 

   setup_target_for_coverage_lcov(
      NAME ${COVERAGE_MAIN}
      EXECUTABLE ctest -j${PROCESSOR_COUNT}
      DEPENDENCIES ${COVERAGE_DEPENDENCIES}
      EXCLUDE  ${COVERAGE_EXCLUDES}
   )

endif()