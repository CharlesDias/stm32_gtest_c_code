#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Middleware/Include/led.h"
#include "gpio_mock.h"

class LedUnitTest : public ::testing::Test
{
protected:
   // Install the mock for the whole test. gMock checks its expectations when
   // the mock is destroyed at the end of the test.
   LedUnitTest()
   {
      setGpioMock(&mock);
   }

   ~LedUnitTest() override
   {
      setGpioMock(nullptr);
   }

   ::testing::StrictMock<GpioMock> mock;
   Gpio_t gpio = {};
   Led_t led = {};
};

TEST_F(LedUnitTest, InitializeWhenCalledWithValidParamsThenSetsFields)
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

// Lets GoogleTest print the parameter values in test output and failure
// messages, instead of the raw bytes of the struct.
void PrintTo(const LedParam &param, std::ostream *os)
{
   *os << (param.activeLevel == LED_ACTIVE_HIGH ? "LED_ACTIVE_HIGH" : "LED_ACTIVE_LOW") << ", "
       << (param.state == LED_STATE_ON ? "LED_STATE_ON" : "LED_STATE_OFF") << " -> "
       << (param.expectedGpioState == GPIO_STATE_SET ? "GPIO_STATE_SET" : "GPIO_STATE_RESET");
}

class LedParamTest : public LedUnitTest,
                     public testing::WithParamInterface<LedParam>
{
};

TEST_P(LedParamTest, WriteWhenCalledThenCallsGpioWritePinWithExpectedState)
{
   const auto &p = GetParam();

   LED_Initialize(&led, &gpio, p.activeLevel);

   EXPECT_CALL(mock, GPIO_WritePin(&gpio, p.expectedGpioState));

   LED_Write(&led, p.state);
}

INSTANTIATE_TEST_SUITE_P(
    LedWriteTests,
    LedParamTest,
    testing::Values(
        // activeLevel,    state,         expectedGpioState, testName
        LedParam{LED_ACTIVE_HIGH, LED_STATE_ON, GPIO_STATE_SET, "ActiveHighOnSetsPin"},
        LedParam{LED_ACTIVE_HIGH, LED_STATE_OFF, GPIO_STATE_RESET, "ActiveHighOffResetsPin"},
        LedParam{LED_ACTIVE_LOW, LED_STATE_ON, GPIO_STATE_RESET, "ActiveLowOnResetsPin"},
        LedParam{LED_ACTIVE_LOW, LED_STATE_OFF, GPIO_STATE_SET, "ActiveLowOffSetsPin"}),
    [](const testing::TestParamInfo<LedParamTest::ParamType> &info)
    {
       return info.param.testName;
    }

);

TEST_F(LedUnitTest, ToggleWhenCalledThenCallsGpioTogglePin)
{
   LED_Initialize(&led, &gpio, LED_ACTIVE_HIGH);

   EXPECT_CALL(mock, GPIO_TogglePin(&gpio)).Times(2);

   LED_Toggle(&led);
   LED_Toggle(&led);
}

#if GTEST_HAS_DEATH_TEST // Make sure death tests are available
// GoogleTest runs test suites named *DeathTest before the others, which keeps
// the fork() used by death tests safe. The alias reuses the LedUnitTest fixture.
using LedDeathTest = LedUnitTest;

// Each death test matches the text of the expected assert, so a crash from
// dereferencing NULL doesn't count as a pass.
TEST_F(LedDeathTest, InitializeWhenPointerIsNullThenFailsAssert)
{
   EXPECT_DEATH(
       {
          LED_Initialize(nullptr, &gpio, LED_ACTIVE_HIGH);
       },
       "me != NULL");
}

TEST_F(LedDeathTest, InitializeWhenAlreadyInitializedThenFailsAssert)
{
   // First init is valid
   LED_Initialize(&led, &gpio, LED_ACTIVE_HIGH);

   EXPECT_DEATH(
       {
          LED_Initialize(&led, &gpio, LED_ACTIVE_HIGH);
       },
       "!me->init");
}

TEST_F(LedDeathTest, InitializeWhenGpioIsNullThenFailsAssert)
{
   EXPECT_DEATH(
       {
          LED_Initialize(&led, nullptr, LED_ACTIVE_HIGH);
       },
       "gpio != NULL");
}

TEST_F(LedDeathTest, WriteWhenNotInitializedThenFailsAssert)
{
   EXPECT_DEATH(
       {
          LED_Write(&led, LED_STATE_ON);
       },
       "me->init");
}

TEST_F(LedDeathTest, WriteWhenPointerIsNullThenFailsAssert)
{
   EXPECT_DEATH(
       {
          LED_Write(nullptr, LED_STATE_ON);
       },
       "me != NULL");
}

TEST_F(LedDeathTest, ToggleWhenNotInitializedThenFailsAssert)
{
   EXPECT_DEATH(
       {
          LED_Toggle(&led);
       },
       "me->init");
}

TEST_F(LedDeathTest, ToggleWhenPointerIsNullThenFailsAssert)
{
   EXPECT_DEATH(
       {
          LED_Toggle(nullptr);
       },
       "me != NULL");
}

#endif // GTEST_HAS_DEATH_TEST
