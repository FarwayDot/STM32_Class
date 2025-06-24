################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/ST/system_stm32f4xx.c 

OBJS += \
./Drivers/CMSIS/ST/system_stm32f4xx.o 

C_DEPS += \
./Drivers/CMSIS/ST/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/ST/%.o Drivers/CMSIS/ST/%.su Drivers/CMSIS/ST/%.cyclo: ../Drivers/CMSIS/ST/%.c Drivers/CMSIS/ST/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F401RETx -DSTM32F4 -DSTM32F401xE -c -I../Inc -I"D:/STM32_Paths/stm32f4/CMSIS/Include" -I"D:/STM32_Paths/stm32f4/CMSIS/Device/ST/STM32F4xx/Include" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-ST

clean-Drivers-2f-CMSIS-2f-ST:
	-$(RM) ./Drivers/CMSIS/ST/system_stm32f4xx.cyclo ./Drivers/CMSIS/ST/system_stm32f4xx.d ./Drivers/CMSIS/ST/system_stm32f4xx.o ./Drivers/CMSIS/ST/system_stm32f4xx.su

.PHONY: clean-Drivers-2f-CMSIS-2f-ST

