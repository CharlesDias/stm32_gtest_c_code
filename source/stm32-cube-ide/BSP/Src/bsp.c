/*
 * bsp.c
 *
 *  Created on: Dec 3, 2022
 *      Author: charlesdias
 */

#include "bsp.h"
#include "stm32f4xx_hal.h"


extern UART_HandleTypeDef huart3;

void BSP_Initialize(void)
{
   printf("BSP_Initialize\r\n");
}

// Function used by printf
int __io_putchar(int ch)
{
   HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 1);
   return 0;
}