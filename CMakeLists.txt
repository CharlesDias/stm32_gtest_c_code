cmake_minimum_required(VERSION 3.21.0)

project(stm32_gtest)

# Default to a Debug build (debug symbols, no optimization) when no build type
# is given. Multi-config generators (Visual Studio, Xcode) choose it at build time.
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)               # Define explicitly to use of the standard C17 declared in the previous line
set(CMAKE_C_EXTENSIONS OFF)                     # Disabling vendor-specific extensions

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)             # Define explicitly to use of the standard C++17 declared in the previous line
set(CMAKE_CXX_EXTENSIONS OFF)                   # Disabling vendor-specific extensions

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)           # Write build/compile_commands.json for IDEs and tools such as clang-tidy

option(ENABLE_COVERAGE      "Enable a Code Coverage build."         OFF)

### CMAKE MODULES
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Doxygen module
include(Docs)

if (ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

message(STATUS "EXTERNAL: Download Google Test Framework release archive...")
include(FetchContent)

# Give extracted files the extraction time, so a new archive always triggers a
# rebuild (policy added in CMake 3.24; older versions skip it)
if (POLICY CMP0135)
   cmake_policy(SET CMP0135 NEW)
endif()

# When updating GoogleTest, change the version in the URL and the SHA256
# (sha256sum of the downloaded archive)
FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.tar.gz
   URL_HASH SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# It must to be declare before the test folder
enable_testing()

# Variables name for target
set(DRIVERS_LIBRARY_TARGET drivers_library)
set(MIDDLEWARE_LIBRARY_TARGET middleware_library)

add_subdirectory(source)
add_subdirectory(tests)
