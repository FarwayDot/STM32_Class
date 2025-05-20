################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../project_name/App/Src/button_task_entry.c \
../project_name/App/Src/task_normal_functions.c 

OBJS += \
./project_name/App/Src/button_task_entry.o \
./project_name/App/Src/task_normal_functions.o 

C_DEPS += \
./project_name/App/Src/button_task_entry.d \
./project_name/App/Src/task_normal_functions.d 


# Each subdirectory must supply rules for building sources it contributes
project_name/App/Src/%.o project_name/App/Src/%.su project_name/App/Src/%.cyclo: ../project_name/App/Src/%.c project_name/App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-project_name-2f-App-2f-Src

clean-project_name-2f-App-2f-Src:
	-$(RM) ./project_name/App/Src/button_task_entry.cyclo ./project_name/App/Src/button_task_entry.d ./project_name/App/Src/button_task_entry.o ./project_name/App/Src/button_task_entry.su ./project_name/App/Src/task_normal_functions.cyclo ./project_name/App/Src/task_normal_functions.d ./project_name/App/Src/task_normal_functions.o ./project_name/App/Src/task_normal_functions.su

.PHONY: clean-project_name-2f-App-2f-Src

