#include "fixture.h"

void GPIO_Initialize(Gpio_t * const me, const GpioPort_t * const port, const GpioPin_t pin)
{
   TestFixture::_halMock->GPIO_Initialize(me, port, pin);
}

void GPIO_TogglePin(const Gpio_t * const me)
{
   TestFixture::_halMock->GPIO_TogglePin(me);
}

void GPIO_WritePin(const Gpio_t * const me, const GpioState_t state)
{
   TestFixture::_halMock->GPIO_WritePin(me, state);
}

GpioState_t GPIO_ReadPin(const Gpio_t * const me)
{
   return TestFixture::_halMock->GPIO_ReadPin(me);
}