/*
 * measurement.h
 *
 *  Created on: May 17, 2023
 *      Author: Oleg Dus
 */

#ifndef LIGHT_SENSOR_H_
#define LIGHT_SENSOR_H_



uint16_t getLightValue(void);
void lightSensorInit(void);
void startLightMeasurment(void);

#endif /* MEASUREMENT_MEASUREMENT_H_ */
