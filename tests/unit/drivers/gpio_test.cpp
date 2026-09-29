#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Drivers/Include/gpio.h"
#include "hal_gpio_mock.h"

using ::testing::InSequence;
using ::testing::Return;

class GpioUnitTest : public ::testing::Test
{
protected:
   // Install the mock for the whole test. gMock checks its expectations when
   // the mock is destroyed at the end of the test.
   GpioUnitTest()
   {
      setHalMock(&mock);
   }

   ~GpioUnitTest() override
   {
      setHalMock(nullptr);
   }

   ::testing::StrictMock<HalMock> mock;

   Gpio_t gpio = {};
   GpioPort_t *gpioPort = (GpioPort_t *)0x40020000;
   GpioPin_t gpioPin = (uint16_t)0x0001;
};

TEST_F(GpioUnitTest, InitializeWhenCalledWithValidParamsThenSetsFields)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   EXPECT_TRUE(gpio.init);
   EXPECT_EQ(gpioPort, gpio.port);
   EXPECT_EQ(gpioPin, gpio.pin);
}

TEST_F(GpioUnitTest, WritePinWhenCalledThenCallsHalWritePin)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   InSequence seq;
   EXPECT_CALL(mock, HAL_GPIO_WritePin((GPIO_TypeDef *)gpioPort, gpioPin, GPIO_PIN_SET));
   EXPECT_CALL(mock, HAL_GPIO_WritePin((GPIO_TypeDef *)gpioPort, gpioPin, GPIO_PIN_RESET));

   GPIO_WritePin(&gpio, GPIO_STATE_SET);
   GPIO_WritePin(&gpio, GPIO_STATE_RESET);
}

TEST_F(GpioUnitTest, ReadPinWhenCalledThenReturnsHalPinState)
{
   GpioState_t returnedState;

   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   EXPECT_CALL(mock, HAL_GPIO_ReadPin((GPIO_TypeDef *)gpioPort, gpioPin)).WillOnce(Return(GPIO_PIN_SET)).WillOnce(Return(GPIO_PIN_RESET));

   returnedState = GPIO_ReadPin(&gpio);
   EXPECT_EQ(GPIO_STATE_SET, returnedState);

   returnedState = GPIO_ReadPin(&gpio);
   EXPECT_EQ(GPIO_STATE_RESET, returnedState);
}

TEST_F(GpioUnitTest, TogglePinWhenCalledThenCallsHalTogglePin)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   EXPECT_CALL(mock, HAL_GPIO_TogglePin((GPIO_TypeDef *)gpioPort, gpioPin));

   GPIO_TogglePin(&gpio);
}

#if GTEST_HAS_DEATH_TEST // Make sure death tests are available
// GoogleTest runs test suites named *DeathTest before the others, which keeps
// the fork() used by death tests safe. The alias reuses the GpioUnitTest fixture.
using GpioDeathTest = GpioUnitTest;

// Each death test matches the text of the expected assert. A catch-all regex
// such as ".*" also accepts a crash from dereferencing NULL, so the test would
// still pass if the assert were removed.
TEST_F(GpioDeathTest, InitializeWhenPointerIsNullThenFailsAssert)
{
   // The function asserts me != NULL, so passing NULL should fail the assert.
   EXPECT_DEATH(
       {
          GPIO_Initialize(nullptr, gpioPort, gpioPin);
       },
       "me != NULL");
}

TEST_F(GpioDeathTest, InitializeWhenAlreadyInitializedThenFailsAssert)
{
   // First init is valid
   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   // Second init with same struct => assertion fail: assert(!me->init);
   EXPECT_DEATH(
       {
          GPIO_Initialize(&gpio, gpioPort, gpioPin);
       },
       "!me->init");
}

TEST_F(GpioDeathTest, InitializeWhenPortIsNullThenFailsAssert)
{
   // me != NULL, me->init is false, but 'port' is NULL => assertion triggers
   EXPECT_DEATH(
       {
          GPIO_Initialize(&gpio, nullptr, gpioPin);
       },
       "port != NULL");
}

TEST_F(GpioDeathTest, WritePinWhenNotInitializedThenFailsAssert)
{
   // me->init is false => assertion triggers
   EXPECT_DEATH(
       {
          GPIO_WritePin(&gpio, GPIO_STATE_SET);
       },
       "me->init");
}

TEST_F(GpioDeathTest, WritePinWhenPointerIsNullThenFailsAssert)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);
   // The function asserts me != NULL, so passing NULL should fail the assert.
   EXPECT_DEATH(
       {
          GPIO_WritePin(nullptr, GPIO_STATE_SET);
       },
       "me != NULL");
}

TEST_F(GpioDeathTest, ReadPinWhenNotInitializedThenFailsAssert)
{
   // me->init is false => assertion triggers
   EXPECT_DEATH(
       {
          GPIO_ReadPin(&gpio);
       },
       "me->init");
}

TEST_F(GpioDeathTest, ReadPinWhenPointerIsNullThenFailsAssert)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);
   // The function asserts me != NULL, so passing NULL should fail the assert.
   EXPECT_DEATH(
       {
          GPIO_ReadPin(nullptr);
       },
       "me != NULL");
}

TEST_F(GpioDeathTest, TogglePinWhenNotInitializedThenFailsAssert)
{
   // me->init is false => assertion triggers
   EXPECT_DEATH(
       {
          GPIO_TogglePin(&gpio);
       },
       "me->init");
}

TEST_F(GpioDeathTest, TogglePinWhenPointerIsNullThenFailsAssert)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);
   // The function asserts me != NULL, so passing NULL should fail the assert.
   EXPECT_DEATH(
       {
          GPIO_TogglePin(nullptr);
       },
       "me != NULL");
}

#endif // GTEST_HAS_DEATH_TEST
