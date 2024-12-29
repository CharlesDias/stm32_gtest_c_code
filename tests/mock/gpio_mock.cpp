#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Drivers/Include/gpio.h"

// Define a mock class with methods corresponding to each GPIO_* function
class GpioMock
{
public:
   MOCK_METHOD(void, GPIO_Initialize, (Gpio_t* const, const GpioPort_t* const, const GpioPin_t));
   MOCK_METHOD(void, GPIO_TogglePin, (const Gpio_t *const));
   MOCK_METHOD(void, GPIO_WritePin, (const Gpio_t *const, GpioState_t));
   MOCK_METHOD(GpioState_t, GPIO_ReadPin, (const Gpio_t *const));
};

// A global pointer to the current mock instance
static GpioMock *g_mock = nullptr;

void setGpioMock(GpioMock *mock)
{
   g_mock = mock;
}

// Now define the real C functions using extern "C", but forward them to our mock:
extern "C"
{

void GPIO_Initialize(Gpio_t* const me, const GpioPort_t* const port, const GpioPin_t pin)
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
   return GPIO_STATE_RESET;
}

} // extern "C"
