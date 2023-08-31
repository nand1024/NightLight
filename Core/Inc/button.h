/*
 * button.h
 *
 *  Created on: May 17, 2023
 *      Author: Oleg Dus
 */

#ifndef BUTTON_H_
#define BUTTON_H_

typedef enum{
    BUTTON_SHORT_CLICK,
    BUTTON_LONG_CLICK,
    BUTTON_NO_CLICK,
} ButtonClickState;



typedef struct {
    uint16_t pressTicks;
    GPIO_TypeDef *port;
    uint16_t pin;
}ButtonStruct;



extern ButtonStruct buttonSwitch;



void buttonInit(ButtonStruct btn[], uint8_t size);
ButtonClickState buttonStateUpdate(ButtonStruct *btn);

#endif /* BUTTON_BUTTON_H_ */
