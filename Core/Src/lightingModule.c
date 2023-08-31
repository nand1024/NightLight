/*
 * lightingModule.c
 *
 *  Created on: May 21, 2023
 *      Author: oleg
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "main.h"
#include "lightingModule.h"
#include "ws2812b_driver.h"
#include "uartDebug.h"
#include "random.h"

typedef struct {
	int32_t r;
	int32_t g;
	int32_t b;
} Color;


//reverse order of LEDs
static void reverseLeds(Color ledArray[], uint8_t size)
{
	Color buffer;

	for (uint8_t i = 0; i < size / 2; i++) {
		buffer = ledArray[i];
		ledArray[i] = ledArray[size - i - 1];
		ledArray[size - i - 1] = buffer;
	}
}


//change of brightness light
static void changeLightIntensive(Color pixels[], uint16_t prewMaxLight, uint16_t nowMaxLight, uint8_t size)
{
	assert(prewMaxLight > 0);
	assert(nowMaxLight > 0);

	for (uint8_t i = 0; i < size; i++) {
		pixels[i].r = pixels[i].r * nowMaxLight / prewMaxLight;
		pixels[i].g = pixels[i].g * nowMaxLight / prewMaxLight;
		pixels[i].b = pixels[i].b * nowMaxLight / prewMaxLight;
	}
}


//creating a sequence of gradation colors
static void colorGradient(Color pixels[], Color fromColor, Color toColor, uint8_t stepDiv, uint8_t size)
{
	assert(stepDiv > 0);

	Color step;

	step.r = (toColor.r - fromColor.r) / stepDiv;
	step.g = (toColor.g - fromColor.g) / stepDiv;
	step.b = (toColor.b - fromColor.b) / stepDiv;

	for (uint8_t i = 0; i < size; i++) {
		pixels[i] = fromColor;
		fromColor.r += step.r;
		fromColor.g += step.g;
		fromColor.b += step.b;
	}
}


static void getPreDefColors(Color colors[TYPE_COLOR_CNT - 1], uint16_t maxLight)
{
	colors[TYPE_COLOR_RED].r = maxLight;         colors[TYPE_COLOR_RED].g = 0;               colors[TYPE_COLOR_RED].b = 0;
	colors[TYPE_COLOR_YELLOW].r = maxLight / 2;  colors[TYPE_COLOR_YELLOW].g = maxLight / 2; colors[TYPE_COLOR_YELLOW].b = 0;
	colors[TYPE_COLOR_GREEN].r = 0;              colors[TYPE_COLOR_GREEN].g = maxLight;      colors[TYPE_COLOR_GREEN].b = 0;
	colors[TYPE_COLOR_CYAN].r = 0;               colors[TYPE_COLOR_CYAN].g = maxLight / 2;   colors[TYPE_COLOR_CYAN].b = maxLight / 2;
	colors[TYPE_COLOR_BLUE].r = 0;               colors[TYPE_COLOR_BLUE].g = 0;              colors[TYPE_COLOR_BLUE].b = maxLight;
	colors[TYPE_COLOR_MAGENTA].r = maxLight / 2; colors[TYPE_COLOR_MAGENTA].g = 0;           colors[TYPE_COLOR_MAGENTA].b = maxLight / 2;
	colors[TYPE_COLOR_NONE].r = 0;               colors[TYPE_COLOR_NONE].g = 0;              colors[TYPE_COLOR_NONE].b = 0;
}


//generate frame of color effect "Flame ball"
static void flameBallFrame(Color pixel[], TypeFlameBall typeFlameBall, TypeColor typeColor[4], uint8_t size, uint16_t maxLight)
{

	assert(typeFlameBall < FLAME_BALL_CNT);

	Color colors[TYPE_COLOR_CNT - 1];
	Color selectColor[4];
	Color endTailColor;
	uint8_t numOfBalls;
	uint8_t lenOfBalls;

	getPreDefColors(colors, maxLight);

	for (uint8_t i = 0; i < 4; i++) {

		assert(typeColor[i] < TYPE_COLOR_CNT);

		if (typeColor[i] != TYPE_COLOR_RANDOM) {
			selectColor[i] = colors[typeColor[i]];
		} else {
			selectColor[i] = colors[getRandom(TYPE_COLOR_CNT - 2)]; //-color random and  - color none
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

	uint8_t index;

	for (uint8_t i = 0; i < numOfBalls; i++) {

		index = i * (size / numOfBalls);

		endTailColor = selectColor[i];

		changeLightIntensive(&endTailColor, maxLight, maxLight / 24, 1);
		//create flame ball with tail
		colorGradient(&pixel[index],
					  endTailColor,
					  selectColor[i],
					  lenOfBalls,
					  lenOfBalls);

		//to fill empty pixels
		for (uint8_t u = lenOfBalls; u < (size / numOfBalls); u++) {
			pixel[index + u] = colors[TYPE_COLOR_NONE];
		}
	}
}


//generate frame of color effect "Rainbow"
void rainbowFrame(Color pixel[], uint8_t size, uint16_t maxLight)
{
	const uint8_t lenGrad = size / 3;
	const uint8_t remainder = size % 3;
	Color colors[TYPE_COLOR_CNT - 1];
	uint8_t sizeOfSteps;
	uint8_t lenOfFragments;
	uint8_t index = 0;

	getPreDefColors(colors, maxLight);

	//calc len gradient and calc steps shades
	lenOfFragments = remainder == 2 ? lenGrad + 1 : lenGrad;
	sizeOfSteps = remainder == 2 ? lenGrad + 1 : lenGrad;
	//create gradient colors from red to green
	colorGradient(&pixel[index],
				  colors[TYPE_COLOR_RED],
				  colors[TYPE_COLOR_GREEN],
				  sizeOfSteps,
				  lenOfFragments);

	//calc len gradient and calc steps shades
	index = remainder == 2 ? lenGrad : lenGrad - 1;
	lenOfFragments = lenGrad * 2;
	sizeOfSteps = lenGrad * 2;
	//create gradient colors from green to blue
	colorGradient(&pixel[index],
				  colors[TYPE_COLOR_GREEN],
				  colors[TYPE_COLOR_BLUE],
				  sizeOfSteps,
				  lenOfFragments);

	//calc len gradient and calc steps shades
	index = remainder == 2 ? lenGrad*2 : lenGrad*2 - 1;
	lenOfFragments = remainder > 0 ? lenGrad*2 + 1 : lenGrad*2;
	sizeOfSteps = remainder > 0 ? lenGrad*2 + 1 : lenGrad*2;
	//create gradient colors from blue to red
	colorGradient(&pixel[index],
			      colors[TYPE_COLOR_BLUE],
				  colors[TYPE_COLOR_RED],
				  sizeOfSteps,
				  lenOfFragments);
}


//generate frame of color effect "Solid"
void solidFrame(Color pixel[], TypeColor typeColor, uint8_t size, uint16_t maxLight)
{

	Color colors[TYPE_COLOR_CNT - 1];
	Color selectColor;

	getPreDefColors(colors, maxLight);

	assert(typeColor < TYPE_COLOR_CNT);

	if (typeColor != TYPE_COLOR_RANDOM) {
		selectColor = colors[typeColor];
	} else {
		selectColor = colors[getRandom(TYPE_COLOR_CNT - 2)]; //-color random and  - color none
	}

	for (uint8_t i = 0; i < size; i++) {
		pixel[i] = selectColor;
	}
}


//generate frame of color effect "Wave"
void waveFrame(Color pixel[], TypeColor typeColor, uint8_t size, uint16_t maxLight)
{
	Color colors[TYPE_COLOR_CNT - 1];
	Color peekWaveColor;
	Color downWaveColor;

	getPreDefColors(colors, maxLight);

	assert(typeColor < TYPE_COLOR_CNT);

	if (typeColor != TYPE_COLOR_RANDOM) {
		peekWaveColor = colors[typeColor];
	} else {
		peekWaveColor = colors[getRandom(TYPE_COLOR_CNT - 2)]; //-color random and  - color none
	}

	downWaveColor = peekWaveColor;

	changeLightIntensive(&downWaveColor, maxLight, maxLight / 24, 1);
	//create gradient colors from maximum light color to minimum light color
	colorGradient(&pixel[0], peekWaveColor, downWaveColor, size/2, size/2);
	//create gradient colors from minimum light color to maximum light color
	colorGradient(&pixel[size/2], downWaveColor, peekWaveColor, size/2, size/2);
}


//create chosed frame of lighting effect
static void createFrame(Color pixel[], EffectSetting *effectSetting, uint8_t size, uint16_t maxLight)
{
	assert(effectSetting->typeFrame < TYPE_FRAME_CNT);

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
			waveFrame(pixel, effectSetting->typeColor[0], size, maxLight);
			break;

		default:
			break;
	}

	if (effectSetting->typeMove == TYPE_MOVE_FRAME_FORWARD) {
		reverseLeds(pixel, size);
	}
}


//move frame by step +-1 led
static void moveFrame(Color pixels[], uint8_t size, TypeMove typeMove)
{
	Color buffer;

	assert(typeMove < TYPE_MOVE_CNT);

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
 * ToDo
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


//calc step for change frame from current frame to target frame
static void calcStepFrame(Color colorNow[],
						  Color colorTarget[],
						  Color colorStep[],
						  uint8_t maxSteps,
						  uint8_t size)
{
	assert(maxSteps > 0);

	Color diff;

	for (uint8_t i = 0; i < size; i++) {

		//calc different
		diff.r = colorTarget[i].r - colorNow[i].r;
		diff.g = colorTarget[i].g - colorNow[i].g;
		diff.b = colorTarget[i].b - colorNow[i].b;

		//calc step
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
	const uint8_t errCalc = 32;
	Color different;

	for (uint8_t i = 0; i < size; i++) {

		//calc different
		different.r = A[i].r - B[i].r;
		different.g = A[i].g - B[i].g;
		different.b = A[i].b - B[i].b;

		//convert to absolute number
		different.r = different.r < 0 ? -different.r : different.r;
		different.g = different.g < 0 ? -different.g : different.g;
		different.b = different.b < 0 ? -different.b : different.b;

		//check equal
		if (different.r > errCalc ||
			different.g > errCalc ||
			different.b > errCalc) {
			return false;
		}
	}

	return true;
}


//this function must be called one per ~5ms for update lighting effect
void lightUpdate(EffectSetting *effectSetting, uint16_t light)
{
	static Color colorNow[LED_SIZE] = {0};
	static Color colorTarget[LED_SIZE];
	static Color colorStep[LED_SIZE];
	static uint16_t maxLight = 0;
	static uint16_t timeEffect = 0;
	const uint8_t steps = 32; //steps for change from current frame to target frame
	Led led[LED_SIZE];

	//check is change light intensive
	if (maxLight != light && maxLight != 0) {
		changeLightIntensive(colorTarget, maxLight, light, LED_SIZE);
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
		led[i].r = colorNow[i].r / RGB_SCALE_MULTIPLE;
		led[i].g = colorNow[i].g / RGB_SCALE_MULTIPLE;
		led[i].b = colorNow[i].b / RGB_SCALE_MULTIPLE;
	}

	//send data to ws28128 leds
	setLight(led);
	timeEffect--;
}
