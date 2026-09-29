#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Middleware/Include/led.h"
#include "hal_gpio_mock.h"

using ::testing::InSequence;

// Integration test: the real LED and GPIO drivers working together, with only
// the STM32 HAL mocked. It checks that an LED command reaches the HAL with the
// right port, pin and pin state.
class LedGpioIntegrationTest : public ::testing::Test
{
protected:
   // Install the mock for the whole test. gMock checks its expectations when
   // the mock is destroyed at the end of the test.
   LedGpioIntegrationTest()
   {
      setHalMock(&mock);
      GPIO_Initialize(&gpio, gpioPort, gpioPin);
   }

   ~LedGpioIntegrationTest() override
   {
      setHalMock(nullptr);
   }

   ::testing::StrictMock<HalMock> mock;

   Gpio_t gpio = {};
   Led_t led = {};
   GpioPort_t *gpioPort = (GpioPort_t *)0x40020400; // GPIOB
   GpioPin_t gpioPin = (uint16_t)0x0080;             // Pin 7, LD2 on the NUCLEO-F446ZE
};

TEST_F(LedGpioIntegrationTest, WriteWhenActiveHighThenDrivesPinHighForOn)
{
   LED_Initialize(&led, &gpio, LED_ACTIVE_HIGH);

   InSequence seq;
   EXPECT_CALL(mock, HAL_GPIO_WritePin((GPIO_TypeDef *)gpioPort, gpioPin, GPIO_PIN_SET));
   EXPECT_CALL(mock, HAL_GPIO_WritePin((GPIO_TypeDef *)gpioPort, gpioPin, GPIO_PIN_RESET));

   LED_Write(&led, LED_STATE_ON);
   LED_Write(&led, LED_STATE_OFF);
}

TEST_F(LedGpioIntegrationTest, WriteWhenActiveLowThenDrivesPinLowForOn)
{
   LED_Initialize(&led, &gpio, LED_ACTIVE_LOW);

   InSequence seq;
   EXPECT_CALL(mock, HAL_GPIO_WritePin((GPIO_TypeDef *)gpioPort, gpioPin, GPIO_PIN_RESET));
   EXPECT_CALL(mock, HAL_GPIO_WritePin((GPIO_TypeDef *)gpioPort, gpioPin, GPIO_PIN_SET));

   LED_Write(&led, LED_STATE_ON);
   LED_Write(&led, LED_STATE_OFF);
}

TEST_F(LedGpioIntegrationTest, ToggleWhenCalledThenTogglesPin)
{
   LED_Initialize(&led, &gpio, LED_ACTIVE_HIGH);

   EXPECT_CALL(mock, HAL_GPIO_TogglePin((GPIO_TypeDef *)gpioPort, gpioPin));

   LED_Toggle(&led);
}
