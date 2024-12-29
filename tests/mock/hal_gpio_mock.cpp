#include "hal_gpio_mock.h"

// A global pointer to the current mock instance
static HalMock* g_halMock = nullptr;

void setHalMock(HalMock* mock)
{
   g_halMock = mock;
}

#ifdef __cplusplus
extern "C" {
#endif

void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
   if (g_halMock)
   {
      g_halMock->HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
   }
}

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
   if (g_halMock)
   {
      g_halMock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
   }
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
   if (g_halMock)
   {
      return g_halMock->HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
   }
   return GPIO_PIN_RESET; // Default fallback
}

#ifdef __cplusplus
} // extern "C"
#endif