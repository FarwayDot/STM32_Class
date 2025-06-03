################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Delay.c \
../Src/GPIO_Config.c \
../Src/RCC.c \
../Src/delay_timer.c \
../Src/main.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timer_capture.c 

OBJS += \
./Src/Delay.o \
./Src/GPIO_Config.o \
./Src/RCC.o \
./Src/delay_timer.o \
./Src/main.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timer_capture.o 

C_DEPS += \
./Src/Delay.d \
./Src/GPIO_Config.d \
./Src/RCC.d \
./Src/delay_timer.d \
./Src/main.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timer_capture.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F401RETx -DSTM32F4 -DSTM32F401xE -c -I../Inc -I"D:/STM32_Paths/stm32f4/CMSIS/Include" -I"D:/STM32_Paths/stm32f4/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/Delay.cyclo ./Src/Delay.d ./Src/Delay.o ./Src/Delay.su ./Src/GPIO_Config.cyclo ./Src/GPIO_Config.d ./Src/GPIO_Config.o ./Src/GPIO_Config.su ./Src/RCC.cyclo ./Src/RCC.d ./Src/RCC.o ./Src/RCC.su ./Src/delay_timer.cyclo ./Src/delay_timer.d ./Src/delay_timer.o ./Src/delay_timer.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/stm32f4xx_it.cyclo ./Src/stm32f4xx_it.d ./Src/stm32f4xx_it.o ./Src/stm32f4xx_it.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timer_capture.cyclo ./Src/timer_capture.d ./Src/timer_capture.o ./Src/timer_capture.su

.PHONY: clean-Src

