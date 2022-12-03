/*
 * Gpio.h
 *
 *  Created on: Nov 24, 2022
 *      Author: charlesdias
 */

#ifndef INCLUDE_GPIO_H_
#define INCLUDE_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t        GpioPort_t;
typedef uint16_t        GpioPin_t;

typedef enum
{
   GPIO_STATE_RESET = 0,
   GPIO_STATE_SET
}GpioState_t;


typedef struct Gpio
{
   bool         init;
   GpioPort_t * port;
   GpioPin_t    pin;
}Gpio_t;


void GPIO_Constructor(Gpio_t * const me, GpioPort_t * port, GpioPin_t pin);
void GPIO_TogglePin(const Gpio_t me);
void GPIO_WritePin(const Gpio_t me, GpioState_t state);
GpioState_t GPIO_ReadPin(const Gpio_t me);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_GPIO_H_ */
