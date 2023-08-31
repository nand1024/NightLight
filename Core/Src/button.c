/*
 * button.c
 *
 *  Created on: May 17, 2023
 *      Author: Oleg Dus
 */

#include "main.h"
#include "button.h"

void buttonInit(ButtonStruct btn[], uint8_t size)
{
	for (uint8_t i = 0; i < size; i++) {
		LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
		if (btn[i].port == GPIOA) {
			LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
		} else if (btn[i].port == GPIOB) {
			LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
		}
		GPIO_InitStruct.Pin = btn[i].pin;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		LL_GPIO_Init(btn[i].port, &GPIO_InitStruct);
	}
}


//check and update state of button
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
