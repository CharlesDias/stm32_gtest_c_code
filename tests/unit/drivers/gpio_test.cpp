#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Drivers/Include/gpio.h"
#include "hal_gpio_mock.cpp"

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class GpioUnitTest : public ::testing::Test
{
protected:
   ::testing::StrictMock<HalMock> *mock;

   Gpio_t gpio = {0};
   GpioPort_t *gpioPort = (GpioPort_t *)0x40020000;
   GpioPin_t gpioPin = (uint16_t)0x0001;

   void SetUp() override
   {
      // Create and install the mock
      mock = new ::testing::StrictMock<HalMock>();
      setHalMock(mock);
   }

   void TearDown() override
   {
      setHalMock(nullptr);
      delete mock;
   }
};

TEST_F(GpioUnitTest, GPIO_Initialize_WhenCalledWithValidParam_ThenInitializesStructField)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   EXPECT_EQ(true, gpio.init);
   EXPECT_EQ(gpioPort, gpio.port);
   EXPECT_EQ(gpioPin, gpio.pin);
}

TEST_F(GpioUnitTest, GPIO_WritePin_WhenCalledWithValidParam_ThenCallsHAL_GPIO_WritePin)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   InSequence seq;
   EXPECT_CALL(*mock, HAL_GPIO_WritePin((GPIO_TypeDef *)gpioPort, gpioPin, (GPIO_PinState)GPIO_STATE_SET)).Times(1);
   EXPECT_CALL(*mock, HAL_GPIO_WritePin((GPIO_TypeDef *)gpioPort, gpioPin, (GPIO_PinState)GPIO_STATE_RESET)).Times(1);

   GPIO_WritePin(&gpio, GPIO_STATE_SET);
   GPIO_WritePin(&gpio, GPIO_STATE_RESET);
}

TEST_F(GpioUnitTest, GPIO_ReadPin_WhenCalledWithValidParam_ThenCallsHAL_GPIO_ReadPin)
{
   GpioState_t returnedState;
   GpioState_t expectStateSet = GPIO_STATE_SET;
   GpioState_t expectStateReset = GPIO_STATE_RESET;

   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   EXPECT_CALL(*mock, HAL_GPIO_ReadPin((GPIO_TypeDef *)gpioPort, gpioPin)).Times(2).WillOnce(Return((GPIO_PinState)expectStateSet)).WillOnce(Return((GPIO_PinState)expectStateReset));

   returnedState = GPIO_ReadPin(&gpio);
   EXPECT_EQ(expectStateSet, returnedState);

   returnedState = GPIO_ReadPin(&gpio);
   EXPECT_EQ(expectStateReset, returnedState);
}

TEST_F(GpioUnitTest, GPIO_TogglePin_WhenCalledWithValidParam_ThenCallsHAL_GPIO_TogglePin)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   EXPECT_CALL(*mock, HAL_GPIO_TogglePin((GPIO_TypeDef *)gpioPort, gpioPin)).Times(1);

   GPIO_TogglePin(&gpio);
}

#if GTEST_HAS_DEATH_TEST // Make sure death tests are available
TEST_F(GpioUnitTest, GPIO_Initialize_WhenPointerIsNull_ThenFailsAssert)
{
   // The function asserts me != NULL, so passing NULL should cause a crash/abort.
   EXPECT_DEATH(
       {
          GPIO_Initialize(nullptr, gpioPort, gpioPin);
       },
       ".*" // A regex checking for assertion message, can be ".*" to ignore
   );
}

TEST_F(GpioUnitTest, GPIO_Initialize_WhenAlreadyInitialized_ThenFailsAssert)
{
   // First init is valid
   GPIO_Initialize(&gpio, gpioPort, gpioPin);

   // Second init with same struct => assertion fail: assert(!me->init);
   EXPECT_DEATH(
       {
          GPIO_Initialize(&gpio, gpioPort, gpioPin);
       },
       ".*");
}

TEST_F(GpioUnitTest, GPIO_Initialize_WhenPortIsNull_ThenFailsAssert)
{
   // me != NULL, me->init is false, but 'port' is NULL => assertion triggers
   EXPECT_DEATH(
       {
          GPIO_Initialize(&gpio, nullptr, gpioPin);
       },
       ".*");
}

TEST_F(GpioUnitTest, Gpio_WritePin_WhenNotInitialized_ThenFailsAssert)
{
   // me->init is false => assertion triggers
   EXPECT_DEATH(
       {
          GPIO_WritePin(&gpio, GPIO_STATE_SET);
       },
       ".*");
}

TEST_F(GpioUnitTest, GPIO_WritePin_WhenPointerIsNull_ThenFailsAssert)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);
   // The function asserts me != NULL, so passing NULL should cause a crash/abort.
   EXPECT_DEATH(
       {
          GPIO_WritePin(nullptr, GPIO_STATE_SET);
       },
       ".*" // A regex checking for assertion message, can be ".*" to ignore
   );
}

TEST_F(GpioUnitTest, GPIO_ReadPin_WhenNotInitialized_ThenFailsAssert)
{
   // me->init is false => assertion triggers
   EXPECT_DEATH(
       {
          GPIO_ReadPin(&gpio);
       },
       ".*");
}

TEST_F(GpioUnitTest, GPIO_ReadPin_WhenPointerIsNull_ThenFailsAssert)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);
   // The function asserts me != NULL, so passing NULL should cause a crash/abort.
   EXPECT_DEATH(
       {
          GPIO_ReadPin(nullptr);
       },
       ".*" // A regex checking for assertion message, can be ".*" to ignore
   );
}

TEST_F(GpioUnitTest, GPIO_TogglePin_WhenNotInitialized_ThenFailsAssert)
{
   // me->init is false => assertion triggers
   EXPECT_DEATH(
       {
          GPIO_TogglePin(&gpio);
       },
       ".*");
}

TEST_F(GpioUnitTest, GPIO_TogglePin_WhenPointerIsNull_ThenFailsAssert)
{
   GPIO_Initialize(&gpio, gpioPort, gpioPin);
   // The function asserts me != NULL, so passing NULL should cause a crash/abort.
   EXPECT_DEATH(
       {
          GPIO_TogglePin(nullptr);
       },
       ".*" // A regex checking for assertion message, can be ".*" to ignore
   );
}

#endif // GTEST_HAS_DEATH_TEST
