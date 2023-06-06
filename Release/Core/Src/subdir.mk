################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/button.c \
../Core/Src/lightEffects.c \
../Core/Src/lightSensor.c \
../Core/Src/lightingModule.c \
../Core/Src/main.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c \
../Core/Src/uartDebug.c \
../Core/Src/ws2812b_driver.c 

OBJS += \
./Core/Src/button.o \
./Core/Src/lightEffects.o \
./Core/Src/lightSensor.o \
./Core/Src/lightingModule.o \
./Core/Src/main.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o \
./Core/Src/uartDebug.o \
./Core/Src/ws2812b_driver.o 

C_DEPS += \
./Core/Src/button.d \
./Core/Src/lightEffects.d \
./Core/Src/lightSensor.d \
./Core/Src/lightingModule.d \
./Core/Src/main.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d \
./Core/Src/uartDebug.d \
./Core/Src/ws2812b_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DSTM32G030xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I/home/oleg/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/STM32G0xx_HAL_Driver/Inc -I/home/oleg/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/CMSIS/Device/ST/STM32G0xx/Include -I/home/oleg/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/CMSIS/Include -IC:/Users/2andn/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/STM32G0xx_HAL_Driver/Inc -IC:/Users/2andn/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/CMSIS/Device/ST/STM32G0xx/Include -IC:/Users/2andn/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/button.cyclo ./Core/Src/button.d ./Core/Src/button.o ./Core/Src/button.su ./Core/Src/lightEffects.cyclo ./Core/Src/lightEffects.d ./Core/Src/lightEffects.o ./Core/Src/lightEffects.su ./Core/Src/lightSensor.cyclo ./Core/Src/lightSensor.d ./Core/Src/lightSensor.o ./Core/Src/lightSensor.su ./Core/Src/lightingModule.cyclo ./Core/Src/lightingModule.d ./Core/Src/lightingModule.o ./Core/Src/lightingModule.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32g0xx_it.cyclo ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g0xx.cyclo ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/system_stm32g0xx.su ./Core/Src/uartDebug.cyclo ./Core/Src/uartDebug.d ./Core/Src/uartDebug.o ./Core/Src/uartDebug.su ./Core/Src/ws2812b_driver.cyclo ./Core/Src/ws2812b_driver.d ./Core/Src/ws2812b_driver.o ./Core/Src/ws2812b_driver.su

.PHONY: clean-Core-2f-Src

