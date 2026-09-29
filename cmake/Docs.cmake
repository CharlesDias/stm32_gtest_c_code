find_package(Doxygen)

if (TARGET Doxygen::doxygen)
   add_custom_target(
      docs
      COMMAND Doxygen::doxygen
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
   )
endif()
