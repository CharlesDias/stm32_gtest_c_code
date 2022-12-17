/*
 * gpio.h
 *
 *  Created on: Dec 3, 2022
 *      Author: charlesdias
 */

#ifndef DRIVERS_INCLUDE_GPIO_H_
#define DRIVERS_INCLUDE_GPIO_H_

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


/**
 * @struct A structure to represent the GPIO pin.
 */
typedef struct Gpio
{
   bool              init; /**< Flag to inform if the structure has been initialized.*/
   const GpioPort_t *port; /**< Port number.*/
   GpioPin_t         pin;  /**< Pin number.*/
}Gpio_t;

/**
 * @brief Initialize the \ref Gpio_t structure.
 * 
 * @param me Pointer to \ref Gpio_t.
 * @param port Port number.
 * @param pin Pin number.
 */
void GPIO_Initialize(Gpio_t * const me, const GpioPort_t * const port, const GpioPin_t pin);
void GPIO_TogglePin(const Gpio_t * const me);
void GPIO_WritePin(const Gpio_t * const me, const GpioState_t state);
GpioState_t GPIO_ReadPin(const Gpio_t * const me);


#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_INCLUDE_GPIO_H_ */
