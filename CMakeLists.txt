cmake_minimum_required(VERSION 3.21.0)

project(stm32_gtest)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)               # Define explicitly to use of the standard C17 declared in the previous line
set(CMAKE_C_EXTENSIONS OFF)                     # Disabling vendor-specific extensions

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)             # Define explicitly to use of the standard C++17 declared in the previous line
set(CMAKE_CXX_EXTENSIONS OFF)                   # Disabling vendor-specific extensions

option(ENABLE_COVERAGE      "Enable a Code Coverage build."         OFF)

### CMAKE MODULES
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
if (ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

message(STATUS "EXTERNAL: Clone Google Test Framework from Git repository...")
include(FetchContent)
FetchContent_Declare(
   googletest
   GIT_REPOSITORY https://github.com/google/googletest.git
   GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# It must to be declare before the test folder
enable_testing()

# Variables name for target
set(LIBRARY_NAME library_gpio)
set(DRIVERS_LIBRARY_TARGET drivers_library)
set(MIDDLEWARE_LIBRARY_TARGET middleware_library)

add_subdirectory(source)
add_subdirectory(tests)