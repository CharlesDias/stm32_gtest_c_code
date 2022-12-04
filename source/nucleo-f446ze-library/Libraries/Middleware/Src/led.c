/*
 * led.c
 *
 *  Created on: 3 de dez de 2022
 *      Author: charlesdias
 */

#include "Middleware/Include/led.h"
#include <stddef.h>
#include <assert.h>

void LED_Initialize(Led_t * const me, const Gpio_t * const gpio, const LedActiveLevel_t activeLevel)
{
   assert(me != NULL);
   assert(!me->init);

   assert(gpio != NULL);

   me->init = true;
   me->gpio = gpio;
   me->activeLevel = activeLevel;
}

void LED_Write(const Led_t * const me, const LedState_t state)
{
   assert(me != NULL);
   assert(me->init);

   GpioState_t gpioState = GPIO_STATE_RESET;

   if (me->activeLevel == LED_ACTIVE_HIGH)
   {
      if(state == LED_STATE_ON)
      {
         gpioState = GPIO_STATE_SET;
      }
   }
   else if (state == LED_STATE_OFF)
   {
      gpioState = GPIO_STATE_SET;
   }

   GPIO_WritePin(me->gpio, gpioState);
}

void LED_Toggle(const Led_t * const me)
{
   assert(me != NULL);
   assert(me->init);

   GPIO_TogglePin(me->gpio);
}
