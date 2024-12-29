#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Drivers/Include/gpio.h"

/**
 * @class GpioMock
 * Mock class for the GPIO_* C functions.
 */
class GpioMock
{
public:
   MOCK_METHOD(void, GPIO_Initialize, (Gpio_t *const me, const GpioPort_t *const port, const GpioPin_t pin));
   MOCK_METHOD(void, GPIO_TogglePin, (const Gpio_t *const me));
   MOCK_METHOD(void, GPIO_WritePin, (const Gpio_t *const me, GpioState_t state));
   MOCK_METHOD(GpioState_t, GPIO_ReadPin, (const Gpio_t *const me));
};

/**
 * @brief Sets the global GpioMock pointer that the extern "C" GPIO_* stubs will call.
 *
 * @param mock Pointer to a GpioMock instance (or nullptr to disable).
 */
void setGpioMock(GpioMock *mock);
