#include "gpio_mock.h"

static GpioMock *g_mock = nullptr;

void setGpioMock(GpioMock *mock)
{
   g_mock = mock;
}

#ifdef __cplusplus
extern "C" {
#endif

void GPIO_Initialize(Gpio_t *const me, const GpioPort_t *const port, const GpioPin_t pin)
{
   if (g_mock)
   {
      g_mock->GPIO_Initialize(me, port, pin);
   }
}

void GPIO_TogglePin(const Gpio_t *const me)
{
   if (g_mock)
   {
      g_mock->GPIO_TogglePin(me);
   }
}

void GPIO_WritePin(const Gpio_t *const me, const GpioState_t state)
{
   if (g_mock)
   {
      g_mock->GPIO_WritePin(me, state);
   }
}

GpioState_t GPIO_ReadPin(const Gpio_t *const me)
{
   if (g_mock)
   {
      return g_mock->GPIO_ReadPin(me);
   }
   return GPIO_STATE_RESET; // Default fallback if no mock is set
}

#ifdef __cplusplus
} // extern "C"
#endif