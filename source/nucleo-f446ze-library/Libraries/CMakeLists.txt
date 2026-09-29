# Compiler warnings for our own library code (not for ST's HAL or GoogleTest)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
   set(LIBRARY_WARNING_FLAGS -Wall -Wextra -Wpedantic)
endif()

# The unit tests check the asserts in our code, so keep them in every build
# type (Release, RelWithDebInfo and MinSizeRel define NDEBUG, which removes
# them). Host builds only: a firmware release build can still remove them.
if (NOT CMAKE_CROSSCOMPILING)
   set(LIBRARY_ASSERT_FLAGS -UNDEBUG)
endif()

add_subdirectory(Drivers)
add_subdirectory(Middleware)
