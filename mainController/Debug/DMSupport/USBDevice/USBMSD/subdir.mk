################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DMSupport/USBDevice/USBMSD/USBMSD.cpp 

OBJS += \
./DMSupport/USBDevice/USBMSD/USBMSD.o 

CPP_DEPS += \
./DMSupport/USBDevice/USBMSD/USBMSD.d 


# Each subdirectory must supply rules for building sources it contributes
DMSupport/USBDevice/USBMSD/%.o: ../DMSupport/USBDevice/USBMSD/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -D__CODE_RED -DCPP_USE_HEAP -DTARGET_RTOS_M4_M7 -DTOOLCHAIN_GCC -DMBED_BUILD_TIMESTAMP=1456087336.46 -DTARGET_CORTEX_M -D__FPU_PRESENT=1 -DTARGET_NXP -DTOOLCHAIN_GCC_CR -DTARGET_M4 -D__MBED__=1 -DTARGET_LIKE_CORTEX_M4 -D__CORTEX_M4 -DTARGET_LPC4088_DM -DARM_MATH_CM4 -DTARGET_LIKE_MBED -DTARGET_LPC408X -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\Bios" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\Display" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\FileSystems" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\Memory" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_FATFileSystem" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_FATFileSystem\ChaN" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_HttpServer" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_HttpServer\Handler" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_USBHost" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_USBHost\USBHost" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_USBHost\USBHost3GModule" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_USBHost\USBHostHID" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_USBHost\USBHostHub" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_USBHost\USBHostMIDI" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_USBHost\USBHostMSD" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\DM_USBHost\USBHostSerial" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\Socket" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip-eth" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip-eth\arch" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip-eth\arch\TARGET_NXP" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip-sys" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip-sys\arch" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\include" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\include\ipv4" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\include\ipv4\lwip" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\include\lwip" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\include\netif" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\netif" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\netif\ppp" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\api" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\core" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\core\ipv4" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\EthernetInterface\lwip\core\snmp" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\USBDevice" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\USBDevice\USBAudio" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\USBDevice\USBDevice" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\USBDevice\USBHID" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\USBDevice\USBMIDI" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\USBDevice\USBMSD" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\USBDevice\USBSerial" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-rpc" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-rtos" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-rtos\rtos" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-rtos\rtx" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M\TARGET_M4" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-rtos\rtx\TARGET_CORTEX_M\TARGET_M4\TOOLCHAIN_GCC" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\api" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\hal" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets\cmsis" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets\cmsis\TARGET_NXP\TARGET_LPC408X\TOOLCHAIN_GCC_CR" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets\hal" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC408X" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\targets\hal\TARGET_NXP\TARGET_LPC408X\TARGET_LPC4088_DM" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMSupport\mbed-src\common" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMemWin" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMemWin\emwin" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMemWin\emwin\emWin_header" -I"C:\Users\Garth\Documents\LPCXpresso_8.0.0_526\workspace\mainController\DMemWin\png" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


