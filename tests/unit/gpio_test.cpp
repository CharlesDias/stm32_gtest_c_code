// #include "GpioMock.cpp"
#include "main_override.h"
#include "Drivers/Include/gpio.h"
#include "hal_gpio_mock.cpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::_;
using ::testing::Return;

class GpioUnitTest : public TestFixture
{
   public:
      GpioUnitTest() {}

      static void SetUpTestSuite()
      {
         // std::cout << "SetUpTestSuite." << std::endl;
      }

      static void TearDownTestSuite()
      {
         // std::cout << "TearDownTestSuite." << std::endl;
      }

      void SetUp() override
      {
         // std::cout << "Set up test cases." << std::endl;
      }

      void TearDown() override
      {
         // std::cout << "Tear Down test cases." << std::endl;
      }
};

TEST_F(GpioUnitTest, GPIO_Initialize_should_initialize_struct_field)
{
   Gpio_t Led = {0};
   GpioPort_t *LedPort = (GpioPort_t *)LD1_GPIO_Port;
   GpioPin_t LedPin = LD1_Pin;

   GPIO_Initialize(&Led, LedPort, LedPin);

   ASSERT_EQ(true, Led.init);
   ASSERT_EQ(LedPort, Led.port);
   ASSERT_EQ(LedPin, Led.pin);
}

TEST_F(GpioUnitTest, GPIO_WritePin_should_CallHAL_GPIO_WritePin_when_PassedValidParameters)
{
   GpioPort_t *expectPort = (GpioPort_t *)LD1_GPIO_Port;
   GpioPin_t expectedPin = LD1_Pin;
   GpioState_t expectStateSet = GPIO_STATE_SET;
   GpioState_t expectStateReset = GPIO_STATE_RESET;
   Gpio_t Led = {.init = true, .port = expectPort, .pin = expectedPin};

   EXPECT_CALL(*_halMock, HAL_GPIO_WritePin((GPIO_TypeDef *)expectPort, expectedPin, (GPIO_PinState)expectStateSet)).Times(1);
   EXPECT_CALL(*_halMock, HAL_GPIO_WritePin((GPIO_TypeDef *)expectPort, expectedPin, (GPIO_PinState)expectStateReset)).Times(1);

   GPIO_WritePin(&Led, GPIO_STATE_SET);
   GPIO_WritePin(&Led, GPIO_STATE_RESET);
}

TEST_F(GpioUnitTest, GPIO_ReadPin_should_CallHAL_GPIO_ReadPin_when_PassedValidParameters)
{
   GpioPort_t *expectedPort = (GpioPort_t *)LD1_GPIO_Port;
   GpioPin_t expectedPin = LD1_Pin;
   GpioState_t expectStateSet = GPIO_STATE_SET;
   GpioState_t expectStateReset = GPIO_STATE_RESET;
   Gpio_t Led = {.init = true, .port = expectedPort, .pin = expectedPin};

   EXPECT_CALL(*_halMock, HAL_GPIO_ReadPin((GPIO_TypeDef *)expectedPort, expectedPin)).Times(2).WillOnce(Return((GPIO_PinState)expectStateSet)).WillOnce(Return((GPIO_PinState)expectStateReset));

   GpioState_t actualStateSet = GPIO_ReadPin(&Led);
   EXPECT_EQ(expectStateSet, actualStateSet);

   GpioState_t actualStateReset = GPIO_ReadPin(&Led);
   EXPECT_EQ(expectStateReset, actualStateReset);
}

TEST_F(GpioUnitTest, GPIO_TogglePin_should_CallHAL_GPIO_TogglePin_when_PassedValidParameters)
{
   GpioPort_t *expectedPort = (GpioPort_t *)LD1_GPIO_Port;
   GpioPin_t expectedPin = LD1_Pin;
   Gpio_t Led = {.init = true, .port = expectedPort, .pin = expectedPin};

   EXPECT_CALL(*_halMock, HAL_GPIO_TogglePin((GPIO_TypeDef *)expectedPort, expectedPin)).Times(1);

   GPIO_TogglePin(&Led);
}