#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __IO volatile

typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;

/**
 * Minimal GPIO_TypeDef layout (fake or from STM32 HAL)
 */
typedef struct
{
  __IO unsigned int MODER;
  __IO unsigned int OTYPER;
  __IO unsigned int OSPEEDR;
  __IO unsigned int PUPDR;
  __IO unsigned int IDR;
  __IO unsigned int ODR;
  __IO unsigned int BSRR;
  __IO unsigned int LCKR;
  __IO unsigned int AFR[2];
} GPIO_TypeDef;

#ifdef __cplusplus
} // extern "C"
#endif

/**
 * @class HalMock
 * Mock class for the STM32 HAL GPIO functions.
 */
class HalMock
{
public:
   virtual ~HalMock() {}

   // HAL Functions
   MOCK_METHOD(void, HAL_GPIO_TogglePin, (GPIO_TypeDef*, uint16_t));
   MOCK_METHOD(void, HAL_GPIO_WritePin,  (GPIO_TypeDef*, uint16_t, GPIO_PinState));
   MOCK_METHOD(GPIO_PinState, HAL_GPIO_ReadPin, (GPIO_TypeDef*, uint16_t));
};

/**
 * @brief Set the global HalMock pointer that the extern "C" functions will call.
 * 
 * @param mock Pointer to a HalMock instance (or nullptr to disable mock calls).
 */
void setHalMock(HalMock* mock);