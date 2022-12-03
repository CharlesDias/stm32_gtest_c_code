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

//
int __io_putchar(int ch)
{
   HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 1);
   return 0;
}

#define __FILE__ (__builtin_strrchr("/"__BASE_FILE__, '/') + 1)

#ifndef TEST_AUTOMATION
// NOTE: If you are using CMSIS, the registers can also be
// accessed through CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk
#define HALT_IF_DEBUGGING()                                     \
   do {                                                         \
      if ((*(volatile uint32_t *)0xE000EDF0) & (1 << 0)) {      \
      __asm("bkpt 1");                                          \
      }                                                         \
   } while (0)

#endif

typedef struct __attribute__((packed)) ContextStateFrame {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t return_address;
  uint32_t xpsr;
} ContextStateFrame_t;

// Disable optimizations for this function so "frame" argument
// does not get optimized away
__attribute__((optimize("O0")))
void BSP_HardFaultHandler(ContextStateFrame_t *frame, uint32_t lr_value)
{
   volatile uint32_t cfsr;
   volatile uint32_t hfsr;
   volatile uint32_t dfsr;
   volatile uint32_t afsr;
   volatile uint32_t bfar;
   volatile uint32_t mmar;

   /* Configurable Fault Status Register */
   /* Consists of MMSR, BFSR and UFSR */
   cfsr = (*((volatile unsigned long *)(0xE000ED28)));

   /* Hard Fault Status Register */
   hfsr = (*((volatile unsigned long *)(0xE000ED2C)));

   /* Debug Fault Status Register */
   dfsr = (*((volatile unsigned long *)(0xE000ED30)));

   /* Auxiliary Fault Status Register */
   afsr = (*((volatile unsigned long *)(0xE000ED3C)));

   /* Read the Fault Address Registers. These may not contain valid values.
    * Check BFARVALID/MMARVALID to see if they are valid values
    * MemManage Fault Address Register*/
   mmar = (*((volatile unsigned long *)(0xE000ED34)));

   /* Bus Fault Address Register */
   bfar = (*((volatile unsigned long *)(0xE000ED38)));

   printf("lr_value: 0x%08X\r\n", (unsigned int)lr_value);
   printf("r0:   0x%08X\r\n", (unsigned int)frame->r0);
   printf("r1:   0x%08X\r\n", (unsigned int)frame->r1);
   printf("r2:   0x%08X\r\n", (unsigned int)frame->r2);
   printf("r3:   0x%08X\r\n", (unsigned int)frame->r3);
   printf("r12:  0x%08X\r\n", (unsigned int)frame->r12);
   printf("lr:   0x%08X\r\n", (unsigned int)frame->lr);
   printf("return_address: 0x%08X\r\n", (unsigned int)frame->return_address);
   printf("xpsr: 0x%08X\r\n", (unsigned int)frame->xpsr);
   printf("cfsr: 0x%08X\r\n", (unsigned int)cfsr);
   printf("hfsr: 0x%08X\r\n", (unsigned int)hfsr);
   printf("dfsr: 0x%08X\r\n", (unsigned int)dfsr);
   printf("afsr: 0x%08X\r\n", (unsigned int)afsr);
   printf("mmar: 0x%08X\r\n", (unsigned int)mmar);
   printf("bfar: 0x%08X\r\n", (unsigned int)bfar);

   // If and only if a debugger is attached, execute a breakpoint
   // instruction so we can take a look at what triggered the fault
   HALT_IF_DEBUGGING();
}
