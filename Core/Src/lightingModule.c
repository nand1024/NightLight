/*
 * lightingModule.c
 *
 *  Created on: May 21, 2023
 *      Author: oleg
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "lightingModule.h"
#include "ws2812b_driver.h"
#include "uartDebug.h"


typedef struct {
	double r;
	double g;
	double b;
} Color;

char debugBuff[128];

static void reversLeds(Color ledArray[], uint8_t size)
{
	Color buffer;

	for (uint8_t i = 0; i < size / 2; i++) {
		buffer = ledArray[i];
		ledArray[i] = ledArray[size - i - 1];
		ledArray[size - i - 1] = buffer;
	}
}



static void changeLightIntensive(Color pixels[], uint8_t size, uint8_t prewMaxLight, uint8_t nowMaxLight)
{
	double coeficient = (double)nowMaxLight / (double)prewMaxLight;
	for (uint8_t i = 0; i < size; i++) {
		pixels[i].r *= coeficient;
		pixels[i].g *= coeficient;
		pixels[i].b *= coeficient;
	}
}



enum {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA,  CLR_CNT};

static void getPreDefColors(Led colors[CLR_CNT], uint8_t maxLight)
{
	colors[RED].r = maxLight;         colors[RED].g = 0;               colors[RED].b = 0;
	colors[YELLOW].r = maxLight / 2;  colors[YELLOW].g = maxLight / 2; colors[YELLOW].b = 0;
	colors[GREEN].r = 0;              colors[GREEN].g = maxLight;      colors[GREEN].b = 0;
	colors[CYAN].r = 0;               colors[CYAN].g = maxLight / 2;   colors[CYAN].b = maxLight / 2;
	colors[BLUE].r = 0;               colors[BLUE].g = 0;              colors[BLUE].b = maxLight;
	colors[MAGENTA].r = maxLight / 2; colors[MAGENTA].g = 0;           colors[MAGENTA].b = maxLight / 2;
}



static void flameBallFrame(Color pixel[], TypeFlameBall typeFlameBall, TypeColor typeColor[4], uint8_t size, uint8_t maxLight)
{
	Led colors[CLR_CNT];
	Led selectColor[4];
	uint8_t numOfBalls;
	uint8_t lenOfBalls;

	getPreDefColors(colors, maxLight);

	for (uint8_t i = 0; i < 4; i++) {
		switch (typeColor[i])
		{
			case TYPE_COLOR_RED:     selectColor[i] = colors[RED]; break;
			case TYPE_COLOR_YELLOW:  selectColor[i] = colors[YELLOW]; break;
			case TYPE_COLOR_GREEN:   selectColor[i] = colors[GREEN]; break;
			case TYPE_COLOR_CYAN:    selectColor[i] = colors[CYAN]; break;
			case TYPE_COLOR_BLUE:    selectColor[i] = colors[BLUE]; break;
			case TYPE_COLOR_MAGENTA: selectColor[i] = colors[MAGENTA]; break;
			case TYPE_COLOR_RANDOM:  selectColor[i] = colors[rand() % CLR_CNT]; break;
			default:
				break;
		}
	}

	switch (typeFlameBall)
	{
		case FLAME_BALL_1_FULL:
			numOfBalls = 1;
			lenOfBalls = size;
			break;

		case FLAME_BALL_1_DIV2LEN:
			numOfBalls = 1;
			lenOfBalls = size / 2;
			break;

		case FLAME_BALL_2_DIV2LEN:
			numOfBalls = 2;
			lenOfBalls = size / 2;
			break;

		case FLAME_BALL_2_DIV4LEN:
			numOfBalls = 2;
			lenOfBalls = size / 4;
			break;

		case FLAME_BALL_4_DIV4LEN:
			numOfBalls = 4;
			lenOfBalls = size / 4;
			break;

		default:
			break;
	}

	uint8_t indexColorBall = 0;

	for (uint8_t i = 0; i < size; i++) {
		if (i % (size / numOfBalls) == 0) {
			uint8_t u;
			for (u = 0; u < lenOfBalls; u++) {
				pixel[i+u].r = selectColor[indexColorBall].r / lenOfBalls * (u+1);
				pixel[i+u].g = selectColor[indexColorBall].g / lenOfBalls * (u+1);
				pixel[i+u].b = selectColor[indexColorBall].b / lenOfBalls * (u+1);
			}
			i += u - 1;
			indexColorBall++;
		} else {
			pixel[i].r = 0;
			pixel[i].g = 0;
			pixel[i].b = 0;
		}
	}
}



void rainbowFrame(Color pixel[], uint8_t size, uint8_t maxLight)
{
	enum {NONE, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, NONE2, COLOR_CNT};

	Led colors[] = {
		[NONE].r = 0,               [NONE].g = 0,              [NONE].b = 0,
		[RED].r = maxLight,         [RED].g = 0,               [RED].b = 0,
		[YELLOW].r = maxLight / 2,  [YELLOW].g = maxLight / 2, [YELLOW].b = 0,
		[GREEN].r = 0,              [GREEN].g = maxLight,      [GREEN].b = 0,
		[CYAN].r = 0,               [CYAN].g = maxLight / 2,   [CYAN].b = maxLight / 2,
		[BLUE].r = 0,               [BLUE].g = 0,              [BLUE].b = maxLight,
		[MAGENTA].r = maxLight / 2, [MAGENTA].g = 0,           [MAGENTA].b = maxLight / 2,
		[NONE2].r = 0,   			[NONE2].g = 0,  		   [NONE2].b = 0,
	};

	for (uint8_t i = 0; i < size; i++) {
		pixel[i].r = colors[i % COLOR_CNT].r;
		pixel[i].g = colors[i % COLOR_CNT].g;
		pixel[i].b = colors[i % COLOR_CNT].b;
	}
}



void solidFrame(Color pixel[], TypeColor typeColor, uint8_t size, uint8_t maxLight)
{

	Led colors[CLR_CNT];
	Led selectColor;

	getPreDefColors(colors, maxLight);

	switch (typeColor)
	{
		case TYPE_COLOR_RED:     selectColor = colors[RED]; break;
		case TYPE_COLOR_YELLOW:  selectColor = colors[YELLOW]; break;
		case TYPE_COLOR_GREEN:   selectColor = colors[GREEN]; break;
		case TYPE_COLOR_CYAN:    selectColor = colors[CYAN]; break;
		case TYPE_COLOR_BLUE:    selectColor = colors[BLUE]; break;
		case TYPE_COLOR_MAGENTA: selectColor = colors[MAGENTA]; break;
		case TYPE_COLOR_RANDOM:  selectColor = colors[rand() % CLR_CNT]; break;
		default:
			break;
	}

	for (uint8_t i = 0; i < size; i++) {
		pixel[i].r = selectColor.r;
		pixel[i].g = selectColor.g;
		pixel[i].b = selectColor.b;
	}
}



void waveFrame(Color pixel[], TypeColor typeColor[2], uint8_t size, uint8_t maxLight)
{
	Led colors[CLR_CNT];
	Led selectColor[2];
	uint8_t halfSize = size / 2;
	double coeficient;

	getPreDefColors(colors, maxLight);

	for (uint8_t i = 0; i < 2; i++) {
		switch (typeColor[i])
		{
			case TYPE_COLOR_RED:     selectColor[i] = colors[RED]; break;
			case TYPE_COLOR_YELLOW:  selectColor[i] = colors[YELLOW]; break;
			case TYPE_COLOR_GREEN:   selectColor[i] = colors[GREEN]; break;
			case TYPE_COLOR_CYAN:    selectColor[i] = colors[CYAN]; break;
			case TYPE_COLOR_BLUE:    selectColor[i] = colors[BLUE]; break;
			case TYPE_COLOR_MAGENTA: selectColor[i] = colors[MAGENTA]; break;
			case TYPE_COLOR_RANDOM:  selectColor[i] = colors[rand() % CLR_CNT]; break;
			default:
				break;
		}
	}

	for (uint8_t i = 0; i < 2; i++) {
		for (uint8_t x = 0; x < halfSize; x++) {
			if (x < (halfSize / 2)) {
				coeficient =  x + 1;
			}else{
				coeficient = (halfSize / 2) - (x - (halfSize / 2));
			}
			pixel[x + i * halfSize].r = selectColor[i].r / (halfSize / 2) * coeficient;
			pixel[x + i * halfSize].g = selectColor[i].g / (halfSize / 2) * coeficient;
			pixel[x + i * halfSize].b = selectColor[i].b / (halfSize / 2) * coeficient;
		}
	}
}



static void createFrame(Color pixel[], EffectSetting *effectSetting, uint8_t size, uint8_t maxLight)
{
	switch (effectSetting->typeFrame) {
		case TYPE_FRAME_FLAME_BALL:
			flameBallFrame(pixel, effectSetting->typeFlameBall, effectSetting->typeColor, size, maxLight);
			break;

		case TYPE_FRAME_RAINBOW:
			rainbowFrame(pixel, size, maxLight);
			break;

		case TYPE_FRAME_SOLID:
			solidFrame(pixel, effectSetting->typeColor[0], size, maxLight);
			break;

		case TYPE_FRAME_WAVE:
			waveFrame(pixel, effectSetting->typeColor, size, maxLight);
			break;

		default:
			break;
	}

	if (effectSetting->typeMove == TYPE_MOVE_FRAME_FORWARD) {
		reversLeds(pixel, size);
	}
}


//move frame by step +-1 led
static void moveFrame(Color pixels[], uint8_t size, TypeMove typeMove)
{
	Color buffer;

	switch (typeMove) {
		case TYPE_MOVE_FRAME_FORWARD:
			buffer = pixels[0];
			for (uint8_t i = 1; i < size; i++) {
				pixels[i-1] = pixels[i];
			}
			pixels[size-1] = buffer;
			break;

		case TYPE_MOVE_FRAME_BACK:
			buffer = pixels[size-1];
			for (uint8_t i = size-1; i > 0; i--) {
				pixels[i] = pixels[i-1];
			}
			pixels[0] = buffer;
			break;

/*
		case TYPE_MOVE_LIGHTING_FORWARD:
			//in progress
			break;

		case TYPE_MOVE_LIGHTING_BACK:
			//in progress
			break;
*/
		default:
			break;
	}
}



static void calcStepFrame(Color colorNow[],
						  Color colorTarget[],
						  Color colorStep[],
						  uint8_t maxSteps,
						  uint8_t size)
{
	Color diff;

	for (uint8_t i = 0; i < size; i++) {
		diff.r = colorTarget[i].r - colorNow[i].r;
		diff.g = colorTarget[i].g - colorNow[i].g;
		diff.b = colorTarget[i].b - colorNow[i].b;

		colorStep[i].r = diff.r / maxSteps;
		colorStep[i].g = diff.g / maxSteps;
		colorStep[i].b = diff.b / maxSteps;
	}
}



static void updateFrame(Color colorNow[], Color colorStep[], uint8_t size)
{
	for (uint8_t i = 0; i < size; i++) {
		colorNow[i].r += colorStep[i].r;
		colorNow[i].g += colorStep[i].g;
		colorNow[i].b += colorStep[i].b;
	}
}



static bool checkEqualFrames(Color A[], Color B[], uint8_t size)
{
	double errCalc = 0.025;
	Color different;

	for (uint8_t i = 0; i < size; i++) {
		different.r = fabs(A[i].r - B[i].r);
		different.g = fabs(A[i].g - B[i].g);
		different.b = fabs(A[i].b - B[i].b);
		if (different.r > errCalc ||
			different.g > errCalc ||
			different.b > errCalc) {
			return false;
		}
	}

	return true;
}



void lightUpdate(EffectSetting *effectSetting, uint8_t light)
{
	static Color colorNow[LED_SIZE] = {0};
	static Color colorTarget[LED_SIZE];
	static Color colorStep[LED_SIZE];
	static uint8_t maxLight = 0;
	static uint16_t timeEffect = 0;

	Led led[LED_SIZE];
	uint8_t steps = 32;

	//check is change light intensive
	if (maxLight != light && maxLight != 0) {
		changeLightIntensive(colorTarget, LED_SIZE, maxLight, light);
		//changeLightIntensive(colorNow, LED_SIZE, maxLight, light);
		calcStepFrame(colorNow, colorTarget, colorStep, steps, LED_SIZE);
	}

	maxLight = light;

	//check need generate new frame if timeEffect is over
	if ((effectSetting->timeLifeColorEffect != 0 && timeEffect == 0)
	    || effectSetting->isNewSet == 1) {
		createFrame(colorTarget, effectSetting, LED_SIZE, maxLight);
		calcStepFrame(colorNow, colorTarget, colorStep, steps, LED_SIZE);
		timeEffect = steps * LED_SIZE * effectSetting->timeLifeColorEffect;
		effectSetting->isNewSet = 0;
	}

	//check is frame changed complete
	if (checkEqualFrames(colorNow, colorTarget, LED_SIZE)) {
		moveFrame(colorTarget, LED_SIZE, effectSetting->typeMove);
		calcStepFrame(colorNow, colorTarget, colorStep, steps, LED_SIZE);
	}

	//change frame to target frame step by step
	updateFrame(colorNow, colorStep, LED_SIZE);

	for (uint8_t i = 0; i < LED_SIZE; i++) {
		led[i].r = colorNow[i].r;
		led[i].g = colorNow[i].g;
		led[i].b = colorNow[i].b;
	}

	//send data to ws28128 leds
	setLight(led);
	timeEffect--;
}
