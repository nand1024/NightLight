/*
 * measurement.c
 *
 *  Created on: May 17, 2023
 *      Author: Oleg Dus
 */
#include "main.h"

#define NUM_OF_MEASURMENTS    8

uint16_t lightVals[NUM_OF_MEASURMENTS];



uint16_t getLightValue(void)
{
    uint16_t res = 0;

    for (uint8_t i = 0; i < NUM_OF_MEASURMENTS; i++) {
        res += lightVals[i];
    }

    return res / NUM_OF_MEASURMENTS;
}



void lightSensorInit(void)
{
    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    #define ADC_CHANNEL_CONF_RDY_TIMEOUT_MS ( 1U)
    #if (USE_TIMEOUT == 1)
    uint32_t Timeout ; /* Variable used for Timeout management */
    #endif /* USE_TIMEOUT */

    ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
    ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
    LL_ADC_Init(ADC1, &ADC_InitStruct);
    LL_ADC_REG_SetSequencerConfigurable(ADC1, LL_ADC_REG_SEQ_CONFIGURABLE);

    #if (USE_TIMEOUT == 1)
    Timeout = ADC_CHANNEL_CONF_RDY_TIMEOUT_MS;
    #endif /* USE_TIMEOUT */
    while (LL_ADC_IsActiveFlag_CCRDY(ADC1) == 0)
     {
    #if (USE_TIMEOUT == 1)
    if (LL_SYSTICK_IsActiveCounterFlag())
     {
    if(Timeout-- == 0)
         {
    Error_Handler();
         }
     }
    #endif /* USE_TIMEOUT */
     }

    LL_ADC_ClearFlag_CCRDY(ADC1);
    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
    ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
    LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
    LL_ADC_SetTriggerFrequencyMode(ADC1, LL_ADC_CLOCK_FREQ_MODE_LOW);
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_2, LL_ADC_SAMPLINGTIME_160CYCLES_5);

    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(ADC1);
    /* Delay for ADC internal voltage regulator stabilization. */
    /* Compute number of CPU cycles to wait for, from delay in us. */
    /* Note: Variable divided by 2 to compensate partially */
    /* CPU processing cycles (depends on compilation optimization). */
    /* Note: If system core clock frequency is below 200kHz, wait time */
    /* is only a few CPU processing cycles. */
    uint32_t wait_loop_index;
    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(wait_loop_index != 0)
    {
        wait_loop_index--;
    }

    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_COMMON_1);
    LL_ADC_Enable(ADC1);

    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)&(ADC1->DR));

    LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_2,
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY
                          | LL_DMA_MODE_NORMAL
                          | LL_DMA_PERIPH_NOINCREMENT
                          | LL_DMA_MEMORY_INCREMENT
                          | LL_DMA_PDATAALIGN_HALFWORD
                          | LL_DMA_MDATAALIGN_HALFWORD
                          | LL_DMA_PRIORITY_HIGH);

    LL_DMAMUX_SetRequestID(DMAMUX1, LL_DMAMUX_CHANNEL_1, LL_DMAMUX_REQ_ADC1);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_LOW);
}



void startLightMeasurment(void)
{
    LL_ADC_REG_StopConversion(ADC1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, NUM_OF_MEASURMENTS);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)&(lightVals[0]));
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
    LL_ADC_REG_StartConversion(ADC1);
}
