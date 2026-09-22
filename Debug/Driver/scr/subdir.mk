################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/scr/clock.c \
../Driver/scr/compare.c \
../Driver/scr/delay.c \
../Driver/scr/gpio.c \
../Driver/scr/uart.c 

OBJS += \
./Driver/scr/clock.o \
./Driver/scr/compare.o \
./Driver/scr/delay.o \
./Driver/scr/gpio.o \
./Driver/scr/uart.o 

C_DEPS += \
./Driver/scr/clock.d \
./Driver/scr/compare.d \
./Driver/scr/delay.d \
./Driver/scr/gpio.d \
./Driver/scr/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/scr/%.o Driver/scr/%.su Driver/scr/%.cyclo: ../Driver/scr/%.c Driver/scr/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"E:/Embedded_C99/STM32f4/stm32f1_base_metal/Driver/inc" -I"D:/Embedded---Firmware/stm32f1_base_metal/Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Driver-2f-scr

clean-Driver-2f-scr:
	-$(RM) ./Driver/scr/clock.cyclo ./Driver/scr/clock.d ./Driver/scr/clock.o ./Driver/scr/clock.su ./Driver/scr/compare.cyclo ./Driver/scr/compare.d ./Driver/scr/compare.o ./Driver/scr/compare.su ./Driver/scr/delay.cyclo ./Driver/scr/delay.d ./Driver/scr/delay.o ./Driver/scr/delay.su ./Driver/scr/gpio.cyclo ./Driver/scr/gpio.d ./Driver/scr/gpio.o ./Driver/scr/gpio.su ./Driver/scr/uart.cyclo ./Driver/scr/uart.d ./Driver/scr/uart.o ./Driver/scr/uart.su

.PHONY: clean-Driver-2f-scr

