/*
 * lightingModule.h
 *
 *  Created on: May 21, 2023
 *      Author: Oleg Dus
 */

#ifndef LIGHTINGMODULE_LIGHTINGMODULE_H_
#define LIGHTINGMODULE_LIGHTINGMODULE_H_
#include "main.h"
#include "ws2812b_driver.h"



typedef enum {
	TYPE_FRAME_FLAME_BALL,
	TYPE_FRAME_RAINBOW,
	TYPE_FRAME_SOLID,
	TYPE_FRAME_WAVE,
	//TYPE_FRAME_SEQUENCE,
	TYPE_FRAME_CNT
}TypeFrame;



typedef enum {
	TYPE_COLOR_NONE,
	TYPE_COLOR_RED,
	TYPE_COLOR_YELLOW,
	TYPE_COLOR_GREEN,
	TYPE_COLOR_CYAN,
	TYPE_COLOR_BLUE,
	TYPE_COLOR_MAGENTA,
	TYPE_COLOR_RANDOM,
	TYPE_COLOR_CNT,
}TypeColor;



typedef enum {
	TYPE_MOVE_FRAME_FORWARD,
	TYPE_MOVE_FRAME_BACK,
	/*
	 *... in progress ...
	TYPE_MOVE_LIGHTING_FORWARD,
	TYPE_MOVE_LIGHTING_BACK,
	*/
	TYPE_MOVE_STOP
}TypeMove;



typedef enum {
	FLAME_BALL_1_FULL,
	FLAME_BALL_1_DIV2LEN,
	FLAME_BALL_2_DIV2LEN,
	FLAME_BALL_2_DIV4LEN,
	FLAME_BALL_4_DIV4LEN,
	FLAME_BALL_CNT,
} TypeFlameBall;



typedef struct {
	uint8_t isNewSet;
	TypeFrame typeFrame;
	TypeMove  typeMove;
	uint16_t timeLifeColorEffect; //if random color generate
	TypeColor typeColor[4]; //LED_SIZE in future
	union{
		TypeFlameBall typeFlameBall;
		//... in progress
	};
}EffectSetting;



void lightUpdate(EffectSetting *effectSetting, uint8_t light);

#endif /* LIGHTINGMODULE_LIGHTINGMODULE_H_ */
