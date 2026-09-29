set(DRIVERS_SOURCES_FILES
   "Src/gpio.c"
)

add_library(${DRIVERS_LIBRARY_TARGET} STATIC
   ${DRIVERS_SOURCES_FILES}
)

# The STM32 HAL headers need the device define; only gpio.c includes them
target_compile_definitions( ${DRIVERS_LIBRARY_TARGET} PRIVATE
   STM32F446xx
)

target_include_directories( ${DRIVERS_LIBRARY_TARGET} PUBLIC
   "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# ST's HAL and CMSIS headers are system headers, so their warnings don't hide
# warnings in our code
target_include_directories( ${DRIVERS_LIBRARY_TARGET} SYSTEM PRIVATE
   ${STM32_CUBE_INCLUDE_FILES}
)

target_compile_options( ${DRIVERS_LIBRARY_TARGET} PRIVATE
   ${LIBRARY_WARNING_FLAGS}
   ${LIBRARY_ASSERT_FLAGS}
)