/*
 * ws2812b_driver.c
 *
 *  Created on: May 17, 2023
 *      Author: Oleg Dus
 */
#include "main.h"
#include "ws2812b_driver.h"

#define LED_TIM_LEN         20
#define LED_TIM_LONG     12
#define LED_TIM_SHORT    6

#define BITS_PER_COLOR     8
#define BITS_PER_LED     24
#define SIZE_TIM_DATA     (BITS_PER_LED * LED_SIZE)



static uint8_t ws2812bTimData[SIZE_TIM_DATA + 2] = {0};
static uint8_t isDmaTransferBusy = 0;



void DMA1_Channel1_IRQHandler(void)
{
    if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
        LL_DMA_ClearFlag_TC1(DMA1);
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
        LL_TIM_DisableCounter(TIM3);
        isDmaTransferBusy = 0;
    }
}



void initLight(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    /* TIM3_CH2 Init */

    LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetPrescaler(TIM3, 0);
    LL_TIM_SetAutoReload(TIM3, LED_TIM_LEN);
    LL_TIM_EnableDMAReq_CC2(TIM3);
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH2);
    LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_OCPOLARITY_HIGH);
    LL_TIM_OC_SetIdleState(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_OCIDLESTATE_LOW);

    /* TIM3 DMA Init */
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_TIM3_CH2);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_VERYHIGH);
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_NORMAL);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_WORD);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&TIM3->CCR2);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&ws2812bTimData[0]);
    LL_DMAMUX_SetRequestID(DMAMUX1, LL_DMAMUX_CHANNEL_0, LL_DMAMUX_REQ_TIM3_CH2);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    NVIC_DisableIRQ(DMA1_Channel1_IRQn);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 2);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    /**TIM3 GPIO Configuration
    PA7   ------> TIM3_CH2
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}



void setLight(Led ledArray[])
{
    uint8_t bitMask [] = {128, 64, 32, 16, 8, 4, 2, 1};

    while(isDmaTransferBusy);
    isDmaTransferBusy = 1;


    for (uint8_t i = 0; i < LED_SIZE; i++) {
        for (uint8_t x = 0; x <  BITS_PER_COLOR; x++) {
            uint16_t indG = BITS_PER_LED * i + x;
            uint16_t indR = BITS_PER_LED * i + x + BITS_PER_COLOR;
            uint16_t indB = BITS_PER_LED * i + x + BITS_PER_COLOR * 2;

            ws2812bTimData[indG] = ledArray[i].g & bitMask[x] ? LED_TIM_LONG : LED_TIM_SHORT;
            ws2812bTimData[indR] = ledArray[i].r & bitMask[x] ? LED_TIM_LONG : LED_TIM_SHORT;
            ws2812bTimData[indB] = ledArray[i].b & bitMask[x] ? LED_TIM_LONG : LED_TIM_SHORT;
        }
    }

    LL_TIM_EnableCounter(TIM3);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&ws2812bTimData[0]);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, SIZE_TIM_DATA + 2);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}
