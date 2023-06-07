/*
 * random.h
 *
 *  Created on: 6 черв. 2023 р.
 *      Author: Oleg Dus
 */

#ifndef INC_RANDOM_H_
#define INC_RANDOM_H_

#include <stdint.h>



void initRandom(uint32_t initVal);
uint32_t getRandom(uint32_t maxVal);

#endif /* INC_RANDOM_H_ */
