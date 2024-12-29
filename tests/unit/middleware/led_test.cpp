#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Middleware/Include/led.h"
#include "gpio_mock.cpp"

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class LedUnitTest : public ::testing::Test
{
protected:
   ::testing::StrictMock<GpioMock> *mock;
   Gpio_t gpio = {0};
   Led_t led = {0};

   void SetUp() override
   {
      // Create and install the mock
      mock = new ::testing::StrictMock<GpioMock>();
      setGpioMock(mock);
   }

   void TearDown() override
   {
      // Unset the mock
      setGpioMock(nullptr);
      delete mock;
   }
};

TEST_F(LedUnitTest, LED_WhenCalled_InitializesStructFields)
{
   LedActiveLevel_t expectedActiveLevel = LED_ACTIVE_HIGH;

   LED_Initialize(&led, &gpio, expectedActiveLevel);

   EXPECT_TRUE(led.init);
   EXPECT_EQ(&gpio, led.gpio);
   EXPECT_EQ(expectedActiveLevel, led.activeLevel);
}

struct LedParam
{
   LedActiveLevel_t activeLevel;
   LedState_t state;
   GpioState_t expectedGpioState;
   const char *testName;
};

class LedParamTest : public LedUnitTest,
                     public testing::WithParamInterface<LedParam>
{
};

TEST_P(LedParamTest, LED_Write_WhenCalled_CallsGpioWritePinWithExpectedState)
{
   const auto &p = GetParam();

   LED_Initialize(&led, &gpio, p.activeLevel);

   EXPECT_CALL(*mock, GPIO_WritePin(&gpio, p.expectedGpioState)).Times(1);

   LED_Write(&led, p.state);
}

INSTANTIATE_TEST_SUITE_P(
    LedWriteTests,
    LedParamTest,
    testing::Values(
        // activeLevel,    state,         expectedGpioState, testName
        LedParam{LED_ACTIVE_HIGH, LED_STATE_ON, GPIO_STATE_SET, "ActiveHigh_STATE_ON_GPIO_SET"},
        LedParam{LED_ACTIVE_HIGH, LED_STATE_OFF, GPIO_STATE_RESET, "ActiveHigh_STATE_OFF_GPIO_RESET"},
        LedParam{LED_ACTIVE_LOW, LED_STATE_ON, GPIO_STATE_RESET, "ActiveLow_STATE_ON_GPIO_RESET"},
        LedParam{LED_ACTIVE_LOW, LED_STATE_OFF, GPIO_STATE_SET, "ActiveLow_STATE_OFF_GPIO_SET"}),
    [](const testing::TestParamInfo<LedParamTest::ParamType> &info)
    {
       return info.param.testName;
    }

);

TEST_F(LedUnitTest, LED_Toggle_WhenCalled_CallsGpioTogglePin)
{
   LED_Initialize(&led, &gpio, LED_ACTIVE_HIGH);

   EXPECT_CALL(*mock, GPIO_TogglePin(&gpio)).Times(2);

   LED_Toggle(&led);
   LED_Toggle(&led);
}