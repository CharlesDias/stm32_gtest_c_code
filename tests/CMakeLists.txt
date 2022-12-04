############################ Test Project ############################
set(SOURCE_FOLDER_PATH "${CMAKE_SOURCE_DIR}/stm32-cube-ide")
set(TEST_FOLDER_PATH "${CMAKE_SOURCE_DIR}/tests")

set(ASSERT_MOCK AssertMock)
add_library(${ASSERT_MOCK}
   ${TEST_FOLDER_PATH}/mock/assert_mock.c
) 

set(UNIT_TEST UnitTest)
add_executable( ${UNIT_TEST}
   ${TEST_FOLDER_PATH}/unit/unit_hello_test.cpp
   ${TEST_FOLDER_PATH}/unit/gpio_test.cpp
)

target_include_directories( ${UNIT_TEST} PRIVATE
   ${TEST_FOLDER_PATH}/header-overrides
   ${TEST_FOLDER_PATH}/mock
)

target_link_libraries( ${UNIT_TEST} PUBLIC
   ${LIBRARY_NAME}
   # ${ASSERT_MOCK}
   GTest::gmock_main
)

set(INTEGRATION_TEST IntegrationTest)
add_executable( ${INTEGRATION_TEST}
${TEST_FOLDER_PATH}/integration/integration_hello_test.cpp
)
target_link_libraries( ${INTEGRATION_TEST}
   GTest::gmock_main
)

############################ Finish Configuration ############################ 

include(GoogleTest)
gtest_discover_tests(
   ${UNIT_TEST}
   ${INTEGRATION_TEST}
)

# Code coverage
if (ENABLE_COVERAGE)
   set(COVERAGE_MAIN "coverage")
   set(COVERAGE_EXCLUDES
      "${PROJECT_SOURCE_DIR}/build/*"
      "${PROJECT_SOURCE_DIR}/stm32-cube-ide/.settings/*"
      "${PROJECT_SOURCE_DIR}/stm32-cube-ide/Core/*"
      "${PROJECT_SOURCE_DIR}/stm32-cube-ide/Debug/*"
      "${PROJECT_SOURCE_DIR}/stm32-cube-ide/Drivers/*"
      "${PROJECT_SOURCE_DIR}/cmake/*"
      # "${PROJECT_SOURCE_DIR}/docs/*"
      # "${PROJECT_SOURCE_DIR}/external/*"
      "${PROJECT_SOURCE_DIR}/tests/*"
      "/usr/include/*"
      )
   setup_target_for_coverage_lcov(
      NAME ${COVERAGE_MAIN}
      EXECUTABLE ${UNIT_TEST}
      DEPENDENCIES ${UNIT_TEST}
   )
endif()