#include "fixture.h"

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
   TestFixture::_halMock->HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
   TestFixture::_halMock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
   return TestFixture::_halMock->HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}
