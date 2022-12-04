#include "main_override.h"
#include "Middleware/Include/led.h"
#include "gpio_mock.cpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

class LedUnitTest : public TestFixture
{
   public:
      Gpio_t gpio = {0};

      LedUnitTest() {}

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
         gpio = {
            .init = false,
            .port = NULL,
            .pin = 0};
         const GpioPort_t *gpioPort = (GpioPort_t *)LD1_GPIO_Port;
         const GpioPin_t gpioPin = LD1_Pin;
         GPIO_Initialize(&gpio, gpioPort, gpioPin);
      }

      void TearDown() override
      {
         // std::cout << "Tear Down test cases." << std::endl;
      }
};

TEST_F(LedUnitTest, LED_Initialize_should_initialize_struct_field)
{
   Led_t Led = {0};
   LedActiveLevel_t expectedActiveLevel = LED_ACTIVE_HIGH;
   
   LED_Initialize(&Led, &gpio, expectedActiveLevel);

   ASSERT_EQ(true, Led.init);
   // ASSERT_EQ(gpio, Led.gpio);
   ASSERT_EQ(expectedActiveLevel, Led.activeLevel);
}

TEST_F(LedUnitTest, LED_Write_when_setAsActiveHigh)
{
   Led_t Led = {0};
   LED_Initialize(&Led, &gpio, LED_ACTIVE_HIGH);

   InSequence seq;
   EXPECT_CALL(*_halMock, GPIO_WritePin(_,GPIO_STATE_SET)).Times(1);
   EXPECT_CALL(*_halMock, GPIO_WritePin(_,GPIO_STATE_RESET)).Times(1);

   LED_Write(&Led, LED_STATE_ON);
   LED_Write(&Led, LED_STATE_OFF);
}

TEST_F(LedUnitTest, LED_Write_when_setAsActiveLow)
{
   Led_t Led = {0};
   LED_Initialize(&Led, &gpio, LED_ACTIVE_LOW);

   InSequence seq;
   EXPECT_CALL(*_halMock, GPIO_WritePin(_,GPIO_STATE_RESET)).Times(1);
   EXPECT_CALL(*_halMock, GPIO_WritePin(_,GPIO_STATE_SET)).Times(1);

   LED_Write(&Led, LED_STATE_ON);
   LED_Write(&Led, LED_STATE_OFF);
}

TEST_F(LedUnitTest, LED_Toggle_should_CallGpio_togglePin)
{
   Led_t Led = {0};
   LED_Initialize(&Led, &gpio, LED_ACTIVE_HIGH);

   EXPECT_CALL(*_halMock, GPIO_TogglePin(_)).Times(2);

   LED_Toggle(&Led);
   LED_Toggle(&Led);
}