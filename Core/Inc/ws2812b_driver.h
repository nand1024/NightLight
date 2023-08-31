/*
 * ws2812b.h
 *
 *  Created on: May 17, 2023
 *      Author: Oleg Dus
 */

#ifndef WS2812B_DRIVER_H_
#define WS2812B_DRIVER_H_

#define LED_SIZE    16

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}Led;

void initLight(void);
void setLight(Led ledArray[]);

#endif /* WS2812B_WS2812B_DRIVER_H_ */
