/*
 * lightEffects.c
 *
 *  Created on: 29 трав. 2023 р.
 *      Author: 2andn
 */

#include "lightEffects.h"


EffectSetting effects[NUM_OF_EFFECTS] = {
		[0].typeFrame = TYPE_FRAME_RAINBOW,
		[0].typeMove = TYPE_MOVE_FRAME_FORWARD,

		[1].typeFrame = TYPE_FRAME_RAINBOW,
		[1].typeMove = TYPE_MOVE_FRAME_BACK,

		[2].typeFrame = TYPE_FRAME_FLAME_BALL,
		[2].typeMove = TYPE_MOVE_FRAME_FORWARD,
		[2].typeFlameBall = FLAME_BALL_1_FULL,
		[2].typeColor[0] = TYPE_COLOR_RANDOM,
		[2].typeColor[1] = TYPE_COLOR_NONE,
		[2].typeColor[2] = TYPE_COLOR_NONE,
		[2].typeColor[3] = TYPE_COLOR_NONE,
		[2].timeLifeColorEffect = 10,

		[3].typeFrame = TYPE_FRAME_FLAME_BALL,
		[3].typeMove = TYPE_MOVE_FRAME_BACK,
		[3].typeFlameBall = FLAME_BALL_1_FULL,
		[3].typeColor[0] = TYPE_COLOR_RANDOM,
		[3].typeColor[1] = TYPE_COLOR_NONE,
		[3].typeColor[2] = TYPE_COLOR_NONE,
		[3].typeColor[3] = TYPE_COLOR_NONE,
		[3].timeLifeColorEffect = 10,

		[4].typeFrame = TYPE_FRAME_FLAME_BALL,
		[4].typeMove = TYPE_MOVE_FRAME_FORWARD,
		[4].typeFlameBall = FLAME_BALL_2_DIV2LEN,
		[4].typeColor[0] = TYPE_COLOR_RANDOM,
		[4].typeColor[1] = TYPE_COLOR_RANDOM,
		[4].typeColor[2] = TYPE_COLOR_NONE,
		[4].typeColor[3] = TYPE_COLOR_NONE,
		[4].timeLifeColorEffect = 10,

		[5].typeFrame = TYPE_FRAME_FLAME_BALL,
		[5].typeMove = TYPE_MOVE_FRAME_BACK,
		[5].typeFlameBall = FLAME_BALL_2_DIV2LEN,
		[5].typeColor[0] = TYPE_COLOR_RANDOM,
		[5].typeColor[1] = TYPE_COLOR_RANDOM,
		[5].typeColor[2] = TYPE_COLOR_NONE,
		[5].typeColor[3] = TYPE_COLOR_NONE,
		[5].timeLifeColorEffect = 10,

		[6].typeFrame = TYPE_FRAME_FLAME_BALL,
		[6].typeMove = TYPE_MOVE_FRAME_FORWARD,
		[6].typeFlameBall = FLAME_BALL_2_DIV2LEN,
		[6].typeColor[0] = TYPE_COLOR_YELLOW,
		[6].typeColor[1] = TYPE_COLOR_BLUE,
		[6].typeColor[2] = TYPE_COLOR_NONE,
		[6].typeColor[3] = TYPE_COLOR_NONE,
		[6].timeLifeColorEffect = 0,

		[7].typeFrame = TYPE_FRAME_FLAME_BALL,
		[7].typeMove = TYPE_MOVE_FRAME_BACK,
		[7].typeFlameBall = FLAME_BALL_2_DIV2LEN,
		[7].typeColor[0] = TYPE_COLOR_YELLOW,
		[7].typeColor[1] = TYPE_COLOR_BLUE,
		[7].typeColor[2] = TYPE_COLOR_NONE,
		[7].typeColor[3] = TYPE_COLOR_NONE,
		[7].timeLifeColorEffect = 0,

		[8].typeFrame = TYPE_FRAME_WAVE,
		[8].typeMove = TYPE_MOVE_FRAME_FORWARD,
		[8].typeColor[0] = TYPE_COLOR_YELLOW,
		[8].typeColor[1] = TYPE_COLOR_MAGENTA,
		[8].timeLifeColorEffect = 0,

		[9].typeFrame = TYPE_FRAME_SOLID,
		[9].typeMove = TYPE_MOVE_STOP,
		[9].typeColor[0] = TYPE_COLOR_RANDOM,
		[9].timeLifeColorEffect = 10,
};
