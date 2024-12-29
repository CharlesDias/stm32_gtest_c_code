#ifndef FIXTURE_H
#define FIXTURE_H

#include "stm32f446xx_override.h"
#include "stm32f4xx_hal_gpio_override.h"
#include "Drivers/Include/gpio.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

class HAL_MOCK
{
   public:
      virtual ~HAL_MOCK() {}

      // HAL Functions
      MOCK_METHOD(void, HAL_GPIO_TogglePin, (GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin));
      MOCK_METHOD(void, HAL_GPIO_WritePin, (GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));
      MOCK_METHOD(GPIO_PinState, HAL_GPIO_ReadPin, (GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin));

      // More mocks can be added here
};

class TestFixture : public ::testing::Test
{
   public:
      TestFixture()
      {
         _halMock.reset(new ::testing::NiceMock<HAL_MOCK>());
      }

      ~TestFixture()
      {
         _halMock.reset();
      }
      virtual void SetUp() {}
      virtual void TearDown() {}

      // pointer for accessing mocked library
      static std::unique_ptr<HAL_MOCK> _halMock;
};

std::unique_ptr<HAL_MOCK> TestFixture::_halMock;

#endif /* FIXTURE_H */