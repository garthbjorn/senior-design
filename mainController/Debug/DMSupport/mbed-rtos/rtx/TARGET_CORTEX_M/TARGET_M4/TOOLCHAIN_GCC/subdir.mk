################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/HAL_CM4.s \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/SVC_Table.s 

OBJS += \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/HAL_CM4.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/SVC_Table.o 


# Each subdirectory must supply rules for building sources it contributes
DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/%.o: ../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/TARGET_M4/TOOLCHAIN_GCC/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__NEWLIB__ -DDEBUG -D__CODE_RED -g3 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__NEWLIB__ -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


