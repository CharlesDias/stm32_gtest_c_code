add_definitions(-DSTM32F446xx) 

set(DRIVERS_SOURCES_FILES
   "Src/gpio.c"
)

add_library(${DRIVERS_LIBRARY_TARGET} STATIC
   ${DRIVERS_SOURCES_FILES}
)

target_include_directories( ${DRIVERS_LIBRARY_TARGET} PUBLIC
   "../../Libraries/"
   PRIVATE
   ${STM32_CUBE_INCLUDE_FILES}   
)