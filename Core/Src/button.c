/*
 * button.c
 *
 *  Created on: May 17, 2023
 *      Author: Oleg Dus
 */

#include "main.h"
#include "button.h"

ButtonStruct buttonSwitch = {
	  .pin = LL_GPIO_PIN_4,
	  .port = GPIOA,
	  .pressTicks = 0
};

void buttonInit(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}



ButtonClickState buttonStateUpdate(ButtonStruct *btn)
{
	const uint16_t tickShortClick = 10;
	const uint16_t tickLongClick = 100;
	ButtonClickState res = BUTTON_NO_CLICK;

	if ((LL_GPIO_ReadInputPort(btn->port) & (btn->pin)) == 0) {
		btn->pressTicks++;
	} else {
		if (btn->pressTicks >= tickLongClick) {
			res = BUTTON_LONG_CLICK;
		} else if (btn->pressTicks >= tickShortClick) {
			res = BUTTON_SHORT_CLICK;
		}
		btn->pressTicks = 0;
	}

	return res;
}
