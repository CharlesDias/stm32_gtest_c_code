/*
 * gpio.c
 *
 *  Created on: Dec 3, 2022
 *      Author: charlesdias
 */

#include "Drivers/Include/gpio.h"
#include "stm32f4xx_hal.h"


void GPIO_Initialize(Gpio_t * const me, GpioPort_t * port, GpioPin_t pin)
{
   assert_param(me != NULL || port != NULL);
   assert_param(!me->init);

   me->init = true;
   me->port = port;
   me->pin = pin;
}

void GPIO_TogglePin(const Gpio_t * const me)
{
   assert_param(me != NULL || me->init);

   HAL_GPIO_TogglePin((GPIO_TypeDef *)me->port, me->pin);
}

void GPIO_WritePin(const Gpio_t * const me, GpioState_t state)
{
   assert_param(me != NULL || me->init);

   if(state == GPIO_STATE_SET)
   {
      HAL_GPIO_WritePin((GPIO_TypeDef *)me->port, me->pin, GPIO_PIN_SET);
   }
   else
   {
      HAL_GPIO_WritePin((GPIO_TypeDef *)me->port, me->pin, GPIO_PIN_RESET);
   }
}

GpioState_t GPIO_ReadPin(const Gpio_t * const me)
{
   assert_param(me != NULL || me->init);

   if(HAL_GPIO_ReadPin((GPIO_TypeDef *)me->port, me->pin))
   {
      return GPIO_STATE_SET;
   }
   else
   {
      return GPIO_STATE_RESET;
   }
}
