set(CURRENT_FOLDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

add_definitions(-DSTM32F446xx) 

set(STM32_CUBE_INCLUDE_FILES
   ${CURRENT_FOLDER_PATH}/Inc/
   # ${CURRENT_FOLDER_PATH}/Core/Inc/
   ${CURRENT_FOLDER_PATH}/Drivers/STM32F4xx_HAL_Driver/Inc/
   ${CURRENT_FOLDER_PATH}/Drivers/CMSIS/Include/
   ${CURRENT_FOLDER_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx/Include/
)

set(LIBRARY_SOURCES_FILES
   "Libraries/Drivers/Src/gpio.c"
)

add_library(${LIBRARY_NAME} STATIC
   ${LIBRARY_SOURCES_FILES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
   "Libraries/"
   PRIVATE
   ${STM32_CUBE_INCLUDE_FILES}
)
