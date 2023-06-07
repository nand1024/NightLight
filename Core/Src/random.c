/*
 * random.c
 *
 *  Created on: 6 черв. 2023 р.
 *      Author: Oleg Dus
 */
#include <stdint.h>

static uint32_t randSequence = 5831;



void initRandom(uint32_t initVal)
{
	randSequence = initVal;
}



uint32_t getRandom(uint32_t maxVal)
{
    randSequence = 8253729 * randSequence + 2396403;
    return randSequence % maxVal;
}
