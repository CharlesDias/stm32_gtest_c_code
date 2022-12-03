set(SOURCE_FOLDER_PATH "${CMAKE_SOURCE_DIR}/stm32-cube-ide")

add_definitions(-DSTM32F446xx) 

set(STM32_CUBE_INCLUDE_FILES
   ${SOURCE_FOLDER_PATH}/Core/Inc/
   ${SOURCE_FOLDER_PATH}/Drivers/STM32F4xx_HAL_Driver/Inc/
   ${SOURCE_FOLDER_PATH}/Drivers/CMSIS/Include/
   ${SOURCE_FOLDER_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx/Include/
)

set(LIBRARY_SOURCES_FILES
   "Library/Src/gpio.c"
)

add_library(${LIBRARY_NAME} STATIC
   ${LIBRARY_SOURCES_FILES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
   "Library/Include/"
   PRIVATE
   ${STM32_CUBE_INCLUDE_FILES}
)
