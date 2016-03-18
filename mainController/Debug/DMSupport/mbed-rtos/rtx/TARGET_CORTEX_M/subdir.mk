################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/HAL_CM.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/RTX_Conf_CM.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_CMSIS.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Event.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_List.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Mailbox.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_MemBox.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Mutex.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Robin.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Semaphore.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_System.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Task.c \
../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Time.c 

OBJS += \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/HAL_CM.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/RTX_Conf_CM.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_CMSIS.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Event.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_List.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Mailbox.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_MemBox.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Mutex.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Robin.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Semaphore.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_System.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Task.o \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Time.o 

C_DEPS += \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/HAL_CM.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/RTX_Conf_CM.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_CMSIS.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Event.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_List.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Mailbox.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_MemBox.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Mutex.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Robin.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Semaphore.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_System.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Task.d \
./DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/rt_Time.d 


# Each subdirectory must supply rules for building sources it contributes
DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/%.o: ../DMSupport/mbed-rtos/rtx/TARGET_CORTEX_M/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -D__CODE_RED -DCPP_USE_HEAP -DTARGET_RTOS_M4_M7 -DTOOLCHAIN_GCC -DMBED_BUILD_TIMESTAMP=1456087336.46 -DTARGET_CORTEX_M -D__FPU_PRESENT=1 -DTARGET_NXP -DTOOLCHAIN_GCC_CR -DTARGET_M4 -D__MBED__=1 -DTARGET_LIKE_CORTEX_M4 -D__CORTEX_M4 -DTARGET_LPC4088_DM -DARM_MATH_CM4 -DTARGET_LIKE_MBED -DTARGET_LPC408X -I"C:\Users\Garth\Documents\senior-design\mainController" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Bios" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Display" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\FileSystems" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\Memory" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_FATFileSystem" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_FATFileSystem\ChaN" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_HttpServer" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_HttpServer\Handler" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHost" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHost3GModule" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostHID" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostHub" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostMIDI" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostMSD" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\DM_USBHost\USBHostSerial" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\Socket" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth\arch" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-eth\arch\TARGET_NXP" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-sys" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip-sys\arch" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\ipv4" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\ipv4\lwip" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\lwip" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\include\netif" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\netif" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\netif\ppp" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\api" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core\ipv4" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\EthernetInterface\lwip\core\snmp" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBAudio" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBDevice" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBHID" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBMIDI" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBMSD" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\USBDevice\USBSerial" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rpc" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtos" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M\TARGET_M4" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M\TARGET_M4\TOOLCHAIN_GCC" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\api" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\hal" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X\TOOLCHAIN_GCC_CR" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC408X" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC408X\TARGET_LPC4088_DM" -I"C:\Users\Garth\Documents\senior-design\mainController\DMSupport\mbed-src\common" -I"C:\Users\Garth\Documents\senior-design\mainController\DMemWin" -I"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\emwin" -I"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\emwin\emWin_header" -I"C:\Users\Garth\Documents\senior-design\mainController\DMemWin\png" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


