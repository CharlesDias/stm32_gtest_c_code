/*
 * led.h
 *
 *  Created on: 3 de dez de 2022
 *      Author: charlesdias
 */

#ifndef MIDDLEWARE_INCLUDE_LED_H_
#define MIDDLEWARE_INCLUDE_LED_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "Drivers/Include/gpio.h"

typedef enum
{
   LED_STATE_OFF,
   LED_STATE_ON,
} LedState_t;

typedef enum
{
   LED_ACTIVE_LOW,
   LED_ACTIVE_HIGH,
} LedActiveLevel_t;

typedef struct Led
{
   bool         init;
   const Gpio_t *gpio;
   bool         activeLevel;
} Led_t;

void LED_Initialize(Led_t * const me, const Gpio_t * const gpio, const LedActiveLevel_t activeLevel);
void LED_Write(const Led_t * const me, const LedState_t state);
void LED_Toggle(const Led_t * const me);

#ifdef __cplusplus
}
#endif

#endif /* MIDDLEWARE_INCLUDE_LED_H_ */
