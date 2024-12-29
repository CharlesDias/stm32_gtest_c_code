#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// #include "stm32f446xx_override.h"
// #include "stm32f4xx_hal_gpio_override.h"

extern "C"
{
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;

typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

} // extern "C"

class HalMock
{
public:
   virtual ~HalMock() {}

   // HAL Functions
   MOCK_METHOD(void, HAL_GPIO_TogglePin, (GPIO_TypeDef*, uint16_t));
   MOCK_METHOD(void, HAL_GPIO_WritePin, (GPIO_TypeDef*, uint16_t, GPIO_PinState));
   MOCK_METHOD(GPIO_PinState, HAL_GPIO_ReadPin, (GPIO_TypeDef*, uint16_t));

   // More mocks can be added here
};

static HalMock *g_halMock = nullptr;

void setHalMock(HalMock *mock)
{
   g_halMock = mock;
}

extern "C"
{
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
   if (g_halMock)
   {
      g_halMock->HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
   }
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
   if (g_halMock)
   {
      g_halMock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
   }
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
   if (g_halMock)
   {
      return g_halMock->HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
   }
   return GPIO_PIN_RESET;
}

} // extern "C"