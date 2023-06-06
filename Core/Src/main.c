
/*
 * main.c
 *
 *  Created on: May 17, 2023
 *      Author: Oleg Dus
 */

#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "lightSensor.h"
#include "uartDebug.h"
#include "ws2812b_driver.h"
#include "lightingModule.h"
#include "button.h"
#include "lightEffects.h"

void SystemClockConfig(void);



static uint8_t setLightPower(uint16_t lightSensorValue) {
	const uint16_t highLight = 400;
	const uint16_t mediumLight = 300;
	const uint16_t lowLight = 200;

	if (lightSensorValue > highLight) {// high
		return 80;
	} else if(lightSensorValue > mediumLight) {// medium
		return 64;
	} else  if(lightSensorValue > lowLight){ // low
		return 48;
	} else { // very low
		return 32;
	}
}



int main(void)
{
	uint8_t maxLight;
	uint8_t modes = 0;

	//initialization
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	NVIC_SetPriority(SysTick_IRQn, 3);
	SystemClockConfig();
	lightSensorInit();
	startLightMeasurment();
	debugInit();
	initLight();
	buttonInit();
	srand(getLightValue());
	effects[modes].isNewSet = 1;

	while (1)
	{
		maxLight = setLightPower(getLightValue());

		if (buttonStateUpdate(&buttonSwitch) == BUTTON_SHORT_CLICK) {
		  if(++modes >= NUM_OF_EFFECTS) modes = 0;
		  effects[modes].isNewSet = 1;
		}

		lightUpdate(&effects[modes], maxLight);
		LL_mDelay(1);
		startLightMeasurment();
		LL_mDelay(4);
	}
}



void SystemClockConfig(void)
{
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(16000000);
  LL_SetSystemCoreClock(16000000);
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
