include(GoogleTest)

############################ Test Project ############################
set(TEST_FOLDER_PATH "${CMAKE_SOURCE_DIR}/tests")

include_directories(
   ${TEST_FOLDER_PATH}/mock
)

######################################################################
# Unit tests for drivers functions
set(DRIVERS_UNIT_TEST DriversUnitTest)
add_executable( ${DRIVERS_UNIT_TEST}
   ${TEST_FOLDER_PATH}/unit/drivers/gpio_test.cpp
   ${TEST_FOLDER_PATH}/mock/hal_gpio_mock.cpp
)

target_link_libraries( ${DRIVERS_UNIT_TEST} PUBLIC
   ${DRIVERS_LIBRARY_TARGET}
   GTest::gmock_main
)
gtest_discover_tests( ${DRIVERS_UNIT_TEST}
   # See
   # https://cmake.org/cmake/help/latest/module/GoogleTest.html#command:gtest_discover_tests
   #
   # XML_OUTPUT_DIR dir
   # New in version 3.18.

   # If specified, the parameter is passed along with --gtest_output=xml: to test executable.
   # The actual file name is the same as the test target, including prefix and suffix. This should
   # be used instead of EXTRA_ARGS --gtest_output=xml to avoid race conditions writing the XML
   # result output when using parallel test execution.

   # XML_OUTPUT_DIR ${TEST_FOLDER_PATH}
)

######################################################################
# Unit tests for middleware functions
set(MIDDLEWARE_UNIT_TEST MiddlewareUnitTest)
add_executable( ${MIDDLEWARE_UNIT_TEST}
   ${TEST_FOLDER_PATH}/unit/middleware/led_test.cpp
   ${TEST_FOLDER_PATH}/mock/gpio_mock.cpp
)

target_link_libraries( ${MIDDLEWARE_UNIT_TEST} PUBLIC
   ${MIDDLEWARE_LIBRARY_TARGET}
   GTest::gmock_main
)
gtest_discover_tests( ${MIDDLEWARE_UNIT_TEST}
)

######################################################################
# Integration test: the LED and GPIO drivers together, with the HAL mocked
set(LED_GPIO_INTEGRATION_TEST LedGpioIntegrationTest)
add_executable( ${LED_GPIO_INTEGRATION_TEST}
   ${TEST_FOLDER_PATH}/integration/led_gpio_integration_test.cpp
   ${TEST_FOLDER_PATH}/mock/hal_gpio_mock.cpp
)

target_link_libraries( ${LED_GPIO_INTEGRATION_TEST} PUBLIC
   ${MIDDLEWARE_LIBRARY_TARGET}
   ${DRIVERS_LIBRARY_TARGET}
   GTest::gmock_main
)
gtest_discover_tests( ${LED_GPIO_INTEGRATION_TEST}
)

######################################################################
# Settings for code coverage
if (ENABLE_COVERAGE)
   set(COVERAGE_MAIN "coverage")

   set(COVERAGE_EXCLUDES
      "${PROJECT_BINARY_DIR}/*"          # GoogleTest, downloaded into the build folder
      # "${PROJECT_SOURCE_DIR}/external/*"
      "${PROJECT_SOURCE_DIR}/tests/*"
      "/usr/include/*"
   )

   set(COVERAGE_DEPENDENCIES 
      ${DRIVERS_UNIT_TEST}
      ${MIDDLEWARE_UNIT_TEST}
      ${LED_GPIO_INTEGRATION_TEST}
   )

   # lcov 2.x (Ubuntu 24.04) stops on GCC end-line mismatches in GoogleTest
   # macros, which don't affect this report. lcov 1.x doesn't know this error
   # name, so only pass it to lcov 2.x.
   execute_process(COMMAND ${LCOV_PATH} --version OUTPUT_VARIABLE LCOV_VERSION_OUTPUT)
   string(REGEX MATCH "[0-9]+\\.[0-9]+" LCOV_VERSION "${LCOV_VERSION_OUTPUT}")
   if (LCOV_VERSION VERSION_GREATER_EQUAL 2.0)
      set(COVERAGE_LCOV_ARGS --ignore-errors mismatch)
   endif()

   # Run the tests in parallel, one job per CPU core
   include(ProcessorCount)
   ProcessorCount(PROCESSOR_COUNT)
   if (PROCESSOR_COUNT EQUAL 0)
      set(PROCESSOR_COUNT 1)
   endif()

   setup_target_for_coverage_lcov(
      NAME ${COVERAGE_MAIN}
      EXECUTABLE ctest -j${PROCESSOR_COUNT}
      DEPENDENCIES ${COVERAGE_DEPENDENCIES}
      EXCLUDE  ${COVERAGE_EXCLUDES}
      LCOV_ARGS ${COVERAGE_LCOV_ARGS}
   )

endif()